#include "statistics_history.h"

#include <cmath>
#include <cstring>

namespace almanac {

static bool finite(double v) { return std::isfinite(v); }

static void copyStr(char* dst, size_t cap, const char* src) {
    if (!dst || cap == 0) return;
    if (!src) { dst[0] = 0; return; }
    size_t n = 0;
    while (src[n] && n + 1 < cap) { dst[n] = src[n]; ++n; }
    dst[n] = 0;
}

static void setMetric(Metric& m, MetricId id, const char* idStr, int hasValue,
                      double value, const char* unit, int currency, int kind,
                      const SimDate& periodStart, const SimDate& periodEnd,
                      const SimDate& asOf, const char* source, int valueClass,
                      int available, int stale) {
    m.schemaVersion = 1;
    m.id = id;
    copyStr(m.idString, SH_ID_LEN, idStr);
    m.hasValue = hasValue;
    m.value = value;
    m.valueFinite = finite(value) ? 1 : 0;
    copyStr(m.unit, SH_UNIT_LEN, unit);
    m.currency = currency;
    m.kind = kind;
    m.periodStart = periodStart;
    m.periodEnd = periodEnd;
    m.asOf = asOf;
    copyStr(m.source, SH_SOURCE_LEN, source);
    m.valueClass = valueClass;
    m.available = available;
    m.stale = stale;
    if (!m.valueFinite) m.available = 0;
}

StatisticsHistory::StatisticsHistory() {
    clearCurrentUnavail_();
}

StatisticsHistory::~StatisticsHistory() = default;

void StatisticsHistory::setSources(const Sources& s) {
    std::lock_guard<std::mutex> lk(mutex_);
    sources_ = s;
}

void StatisticsHistory::clearCurrentUnavail_() {
    current_.schemaVersion = 1;
    current_.metricCount = 0;
    current_.coherent = 1;
    current_.sequence = sequence_;
    for (size_t i = 0; i < SH_MAX_METRICS; ++i) {
        Metric& m = current_.metrics[i];
        m = Metric{};
    }
}

void StatisticsHistory::reset(ResetReason /*reason*/) {
    std::lock_guard<std::mutex> lk(mutex_);
    clearCurrentUnavail_();
    // clear history ring
    ringCount_ = 0;
    ringWrite_ = 0;
    for (size_t i = 0; i < SH_HISTORY_CAPACITY; ++i) ring_[i] = Snapshot{};
    lastAsOf_ = SimDate{};
    hasLastAsOf_ = false;
    lastCurrency_ = -1;
}

int StatisticsHistory::cmpDate_(const SimDate& a, const SimDate& b) {
    if (a.year != b.year) return (a.year < b.year) ? -1 : 1;
    if (a.month != b.month) return (a.month < b.month) ? -1 : 1;
    if (a.day != b.day) return (a.day < b.day) ? -1 : 1;
    return 0;
}

bool StatisticsHistory::isBackward_(const SimDate& asOf) const {
    if (!hasLastAsOf_) return false;
    return cmpDate_(asOf, lastAsOf_) < 0;
}

void StatisticsHistory::buildCurrent_(const SimDate& asOf, int currency) {
    // currency symbol
    const char* sym = nullptr;
    if (sources_.currency.getSymbol) sym = sources_.currency.getSymbol(sources_.currency.userData);
    if (sym) copyStr(current_.currencySymbol, SH_SYMBOL_LEN, sym);
    else {
        if (currency == 1) copyStr(current_.currencySymbol, SH_SYMBOL_LEN, "USD");
        else copyStr(current_.currencySymbol, SH_SYMBOL_LEN, "RUB");
    }
    current_.currency = currency;
    current_.asOf = asOf;

    // remove unused helpers
    SimDate flowStart = asOf;
    flowStart.day = 1;

    size_t mi = 0;
    auto add = [&](MetricId id, const char* idStr, int hasValue, double value,
                   const char* unit, int cur, int kind, const SimDate& pstart,
                   const SimDate& pend, const char* source, int vc, int avail, int stale) {
        if (mi >= SH_MAX_METRICS) return;
        setMetric(current_.metrics[mi], id, idStr, hasValue, value, unit, cur, kind,
                  pstart, pend, asOf, source, vc, avail, stale);
        ++mi;
    };

    // --- Overview ---
    // treasury
    {
        double v = 0.0; int ok = 0;
        if (sources_.currency.getTreasury) {
            double t = 0.0;
            if (sources_.currency.getTreasury(sources_.currency.userData, currency, &t) && finite(t)) { v = t; ok = 1; }
        }
        add(MID_OVERVIEW_TREASURY, "overview.treasury", ok, v,
            currency == 1 ? "USD" : "RUB", currency, MK_STOCK, asOf, asOf,
            "currency.getTreasury", VC_ACTUAL, ok, 0);
    }
    // wallets total
    {
        double v = 0.0; int ok = 0;
        if (sources_.currency.getTotalWallets) {
            double t = 0.0;
            if (sources_.currency.getTotalWallets(sources_.currency.userData, &t) && finite(t)) { v = t; ok = 1; }
        }
        add(MID_OVERVIEW_WALLETS_TOTAL, "overview.wallets_total", ok, v,
            currency == 1 ? "USD" : "RUB", currency, MK_STOCK, asOf, asOf,
            "currency.getTotalWalletBalance", VC_ACTUAL, ok, 0);
        // citizen wallets metric alias
    }
    // enterprise capital total
    {
        double v = 0.0; int ok = 0;
        if (sources_.enterprise.getCapitalSum) {
            double t = 0.0;
            if (sources_.enterprise.getCapitalSum(sources_.enterprise.userData, &t) && finite(t)) { v = t; ok = 1; }
        }
        add(MID_OVERVIEW_ENTERPRISE_CAPITAL_TOTAL, "overview.enterprise_capital_total", ok, v,
            currency == 1 ? "USD" : "RUB", currency, MK_STOCK, asOf, asOf,
            "enterprise_base capital sum", VC_ACTUAL, ok, 0);
    }
    // investment pool
    {
        double v = 0.0; int ok = 0;
        if (sources_.enterprise.getPool) {
            double t = 0.0;
            if (sources_.enterprise.getPool(sources_.enterprise.userData, &t) && finite(t)) { v = t; ok = 1; }
        }
        add(MID_OVERVIEW_INVESTMENT_POOL, "overview.investment_pool", ok, v,
            currency == 1 ? "USD" : "RUB", currency, MK_STOCK, asOf, asOf,
            "enterprise_base.getPrivateInvestmentPool", VC_ACTUAL, ok, 0);
    }
    // enterprise count
    {
        size_t v = 0; int ok = 0;
        if (sources_.enterprise.getCount) {
            size_t t = 0;
            if (sources_.enterprise.getCount(sources_.enterprise.userData, &t)) { v = t; ok = 1; }
        }
        add(MID_OVERVIEW_ENTERPRISE_COUNT, "overview.enterprise_count", ok, (double)v,
            "count", -1, MK_STOCK, asOf, asOf,
            "enterprise_base.getEnterpriseCount", VC_ACTUAL, ok, 0);
    }
    // money supply: sum treasury + wallets + enterprise + pool (same currency, all available)
    {
        double tr = 0, wa = 0, en = 0, po = 0;
        int oktr = 0, okwa = 0,oken = 0,okpo = 0;
        if (sources_.currency.getTreasury) {
            double t=0;
            if (sources_.currency.getTreasury(sources_.currency.userData, currency, &t) && finite(t)) { tr=t; oktr=1; }
        }
        if (sources_.currency.getTotalWallets) {
            double t=0;
            if (sources_.currency.getTotalWallets(sources_.currency.userData, &t) && finite(t)) { wa=t; okwa=1; }
        }
        if (sources_.enterprise.getCapitalSum) {
            double t=0;
            if (sources_.enterprise.getCapitalSum(sources_.enterprise.userData, &t) && finite(t)) { en=t; oken=1; }
        }
        if (sources_.enterprise.getPool) {
            double t=0;
            if (sources_.enterprise.getPool(sources_.enterprise.userData, &t) && finite(t)) { po=t; okpo=1; }
        }
        int ok = oktr && okwa && oken && okpo;
        double v = ok ? (tr + wa + en + po) : 0.0;
        add(MID_OVERVIEW_MONEY_SUPPLY, "overview.money_supply", ok, v,
            currency == 1 ? "USD" : "RUB", currency, MK_STOCK, asOf, asOf,
            "sum treasury+wallets+enterprise+pool (actual only)", VC_ACTUAL, ok, 0);
    }

    // --- Money Flow (monthly flows) ---
    auto flowMetric = [&](MetricId id, const char* idStr, int (*fn)(void*, double*),
                          void* ud, const char* src) {
        double v = 0.0; int ok = 0;
        if (fn) {
            double t = 0.0;
            if (fn(ud, &t) && finite(t)) { v = t; ok = 1; }
        }
        add(id, idStr, ok, v, currency == 1 ? "USD" : "RUB", currency, MK_FLOW,
            flowStart, asOf, src, VC_ACTUAL, ok, 0);
    };
    flowMetric(MID_FLOW_WAGES, "flow.wages", sources_.flow.getWages, sources_.flow.userData, "flow.getWages");
    flowMetric(MID_FLOW_PENSIONS, "flow.pensions", sources_.flow.getPensions, sources_.flow.userData, "flow.getPensions");
    flowMetric(MID_FLOW_WELFARE, "flow.welfare", sources_.flow.getWelfare, sources_.flow.userData, "flow.getWelfare");
    flowMetric(MID_FLOW_STATE_STORE_SALES, "flow.state_store_sales", sources_.flow.getStateStoreSales, sources_.flow.userData, "flow.getStateStoreSales");
    flowMetric(MID_FLOW_WHOLESALE_B2B, "flow.wholesale_b2b", sources_.flow.getWholesale, sources_.flow.userData, "flow.getWholesale");
    flowMetric(MID_FLOW_VAT, "flow.vat", sources_.flow.getVat, sources_.flow.userData, "flow.getVat");
    flowMetric(MID_FLOW_PROFIT_TAX, "flow.profit_tax", sources_.flow.getProfitTax, sources_.flow.userData, "flow.getProfitTax");
    flowMetric(MID_FLOW_PROPERTY_TAX, "flow.property_tax", sources_.flow.getPropertyTax, sources_.flow.userData, "flow.getPropertyTax");
    flowMetric(MID_FLOW_NET_TREASURY, "flow.net_treasury", sources_.flow.getNetTreasury, sources_.flow.userData, "flow.getNetTreasury");
    flowMetric(MID_FLOW_UNEXPLAINED, "flow.unexplained", sources_.flow.getUnexplained, sources_.flow.userData, "flow.getUnexplained");

    // --- Private Economy ---
    {
        double sales=0,wages=0,taxes=0,pool=0; int ok=0;
        if (sources_.enterprise.getMonthlySummary) {
            double s=0,w=0,t=0,p=0;
            if (sources_.enterprise.getMonthlySummary(sources_.enterprise.userData,&s,&w,&t,&p)) {
                if (finite(s)&&finite(w)&&finite(t)&&finite(p)) { sales=s;wages=w;taxes=t;pool=p; ok=1; }
            }
        }
        add(MID_PRIVATE_SALES, "private.sales", ok, sales, currency==1?"USD":"RUB", currency, MK_FLOW, flowStart, asOf, "enterprise.getMonthlyEconomySummary", VC_ACTUAL, ok, 0);
        add(MID_PRIVATE_WAGES, "private.wages", ok, wages, currency==1?"USD":"RUB", currency, MK_FLOW, flowStart, asOf, "enterprise.getMonthlyEconomySummary", VC_ACTUAL, ok, 0);
        add(MID_PRIVATE_TAXES, "private.taxes", ok, taxes, currency==1?"USD":"RUB", currency, MK_FLOW, flowStart, asOf, "enterprise.getMonthlyEconomySummary", VC_ACTUAL, ok, 0);
        add(MID_PRIVATE_POOL_INFLOW, "private.pool_inflow", ok, pool, currency==1?"USD":"RUB", currency, MK_FLOW, flowStart, asOf, "enterprise.getMonthlyEconomySummary", VC_ACTUAL, ok, 0);
    }
    {
        size_t v=0; int ok=0;
        if (sources_.enterprise.getCount) {
            size_t t=0;
            if (sources_.enterprise.getCount(sources_.enterprise.userData,&t)) { v=t; ok=1; }
        }
        add(MID_PRIVATE_ENTERPRISE_COUNT, "private.enterprise_count", ok, (double)v, "count", -1, MK_STOCK, asOf, asOf, "enterprise.getEnterpriseCount", VC_ACTUAL, ok, 0);
    }
    {
        size_t v=0; int ok=0;
        if (sources_.enterprise.getMonthlyEvictions) {
            size_t t=0;
            if (sources_.enterprise.getMonthlyEvictions(sources_.enterprise.userData,&t)) { v=t; ok=1; }
        }
        add(MID_PRIVATE_EVICTIONS, "private.evictions", ok, (double)v, "count", -1, MK_FLOW, flowStart, asOf, "enterprise.getMonthlyEvictionCount", VC_ACTUAL, ok, 0);
    }
    {
        double v=0.0; int ok=0;
        if (sources_.enterprise.getRentMonthly) {
            double t=0.0;
            if (sources_.enterprise.getRentMonthly(sources_.enterprise.userData,&t) && finite(t)) { v=t; ok=1; }
        }
        add(MID_PRIVATE_RENT_MONTHLY, "private.rent_monthly", ok, v, currency==1?"USD":"RUB", currency, MK_STOCK, asOf, asOf, "enterprise.getResidentialRentMonthly", VC_ACTUAL, ok, 0);
    }

    // --- Prices & Citizens ---
    auto priceMetric = [&](MetricId id, const char* idStr, int (*fn)(void*, int, double*), const char* src) {
        double v=0.0; int ok=0;
        if (fn) {
            double t=0.0;
            if (fn(sources_.pricing.userData, currency, &t) && finite(t) && t > 0.0) { v=t; ok=1; }
        }
        add(id, idStr, ok, v, currency==1?"USD/ton":"RUB/ton", currency, MK_STOCK, asOf, asOf, src, VC_ACTUAL, ok, 0);
    };
    priceMetric(MID_PRICE_FOOD, "price.food", sources_.pricing.getFoodPrice, "pricing.getFood price");
    priceMetric(MID_PRICE_MEAT, "price.meat", sources_.pricing.getMeatPrice, "pricing.getMeat price");
    priceMetric(MID_PRICE_CLOTHES, "price.clothes", sources_.pricing.getClothesPrice, "pricing.getClothes price");
    priceMetric(MID_PRICE_ELECTRONICS, "price.electronics", sources_.pricing.getElectronicsPrice, "pricing.getEletronics price");

    // CPI
    {
        double v=0.0; int ok=0;
        if (sources_.pricing.getCpiBasket) {
            double t=0.0;
            if (sources_.pricing.getCpiBasket(sources_.pricing.userData, 0.0, &t) && finite(t) && t > 0.0) { v=t; ok=1; }
        }
        add(MID_CPI_DAILY_BASKET, "price.cpi_daily_basket", ok, v, currency==1?"USD/day":"RUB/day", currency, MK_STOCK, asOf, asOf, "pricing.getCpiDailyBasket", VC_ACTUAL, ok, 0);
    }
    // market mult food
    {
        double v=0.0; int ok=0;
        if (sources_.pricing.getMarketMultFood) {
            double t=0.0;
            if (sources_.pricing.getMarketMultFood(sources_.pricing.userData, &t) && finite(t)) { v=t; ok=1; }
        }
        add(MID_MARKET_MULT_FOOD, "price.market_mult_food", ok, v, "ratio", -1, MK_STOCK, asOf, asOf, "pricing.getMarketMultiplier", VC_ACTUAL, ok, 0);
    }
    // citizen wallets total (alias)
    {
        double v=0.0; int ok=0;
        if (sources_.currency.getTotalWallets) {
            double t=0.0;
            if (sources_.currency.getTotalWallets(sources_.currency.userData, &t) && finite(t)) { v=t; ok=1; }
        }
        add(MID_CITIZEN_WALLETS_TOTAL, "citizen.wallets_total", ok, v, currency==1?"USD":"RUB", currency, MK_STOCK, asOf, asOf, "currency.getTotalWalletBalance", VC_ACTUAL, ok, 0);
    }
    // citizen count: no source available -> unavailable (documented missing source)
    add(MID_CITIZEN_COUNT, "citizen.count", 0, 0.0, "count", -1, MK_STOCK, asOf, asOf, "UNAVAILABLE: no citizen count source", VC_ACTUAL, 0, 0);

    // --- Logistics & Production ---
    {
        double v=0.0; int ok=0;
        if (sources_.logistics.getFuelConsumed) {
            double t=0.0;
            if (sources_.logistics.getFuelConsumed(sources_.logistics.userData, &t) && finite(t)) { v=t; ok=1; }
        }
        add(MID_LOGISTICS_FUEL_CONSUMED, "logistics.fuel_consumed", ok, v, "fuel", -1, MK_FLOW, flowStart, asOf, "private_logistics (dry-run)", VC_DRY_RUN, ok, 0);
    }
    {
        double v=0.0; int ok=0;
        if (sources_.logistics.getTripCount) {
            double t=0.0;
            if (sources_.logistics.getTripCount(sources_.logistics.userData, &t) && finite(t)) { v=t; ok=1; }
        }
        add(MID_LOGISTICS_TRIP_COUNT, "logistics.trip_count", ok, v, "count", -1, MK_FLOW, flowStart, asOf, "private_logistics (dry-run)", VC_DRY_RUN, ok, 0);
    }
    {
        double v=0.0; int ok=0;
        if (sources_.logistics.getTonsMoved) {
            double t=0.0;
            if (sources_.logistics.getTonsMoved(sources_.logistics.userData, &t) && finite(t)) { v=t; ok=1; }
        }
        add(MID_LOGISTICS_TONS_MOVED, "logistics.tons_moved", ok, v, "tons", -1, MK_FLOW, flowStart, asOf, "private_logistics (dry-run)", VC_DRY_RUN, ok, 0);
    }
    {
        int dry = 1; int ok = 0;
        if (sources_.logistics.isDryRun) {
            int t = 0;
            if (sources_.logistics.isDryRun(sources_.logistics.userData, &t)) { dry = t; ok = 1; }
        } else {
            // default documented: billing remains 0, dry_run remains 1
            dry = 1; ok = 1;
        }
        add(MID_LOGISTICS_DRY_RUN_FLAG, "logistics.dry_run", 1, (double)dry, "flag", -1, MK_STOCK, asOf, asOf, "private_logistics config (dry_run=1)", VC_ACTUAL, 1, 0);
        (void)ok;
    }
    // production output: no source -> unavailable
    add(MID_PRODUCTION_OUTPUT, "production.output", 0, 0.0, "tons", -1, MK_FLOW, flowStart, asOf, "UNAVAILABLE: no production source", VC_ACTUAL, 0, 0);

    current_.metricCount = mi;
    current_.sequence = ++sequence_;
}

bool StatisticsHistory::update(const SimDate& asOf) {
    std::lock_guard<std::mutex> lk(mutex_);
    // backward time detection
    bool backward = isBackward_(asOf);
    // currency
    int currency = 0;
    if (sources_.currency.getActiveCurrency) {
        int c = sources_.currency.getActiveCurrency(sources_.currency.userData);
        currency = (c == 1) ? 1 : 0;
    }
    bool currencyChanged = hasLastAsOf_ && (currency != lastCurrency_) && (lastCurrency_ != -1);

    if (backward) {
        // reset
        clearCurrentUnavail_();
        ringCount_ = 0;
        ringWrite_ = 0;
        for (size_t i = 0; i < SH_HISTORY_CAPACITY; ++i) ring_[i] = Snapshot{};
        lastAsOf_ = asOf;
        hasLastAsOf_ = true;
        lastCurrency_ = currency;
        buildCurrent_(asOf, currency);
        return false;
    }
    if (currencyChanged) {
        clearCurrentUnavail_();
        ringCount_ = 0;
        ringWrite_ = 0;
        for (size_t i = 0; i < SH_HISTORY_CAPACITY; ++i) ring_[i] = Snapshot{};
    }
    lastAsOf_ = asOf;
    hasLastAsOf_ = true;
    lastCurrency_ = currency;
    buildCurrent_(asOf, currency);
    return true;
}

void StatisticsHistory::recordHistory(const SimDate& asOf) {
    std::lock_guard<std::mutex> lk(mutex_);
    Snapshot s = current_;
    s.asOf = asOf;
    s.sequence = current_.sequence;
    ring_[ringWrite_] = s;
    ringWrite_ = (ringWrite_ + 1) % SH_HISTORY_CAPACITY;
    if (ringCount_ < SH_HISTORY_CAPACITY) ++ringCount_;
}

Snapshot StatisticsHistory::getSnapshot() const {
    std::lock_guard<std::mutex> lk(mutex_);
    Snapshot s = current_;
    return s;
}

size_t StatisticsHistory::historyCount() const {
    std::lock_guard<std::mutex> lk(mutex_);
    return ringCount_;
}

size_t StatisticsHistory::readHistory(Snapshot* dest, size_t destCapacity) const {
    std::lock_guard<std::mutex> lk(mutex_);
    if (!dest || destCapacity == 0) return 0;
    size_t n = ringCount_;
    if (n > destCapacity) n = destCapacity;
    if (n > SH_HISTORY_CAPACITY) n = SH_HISTORY_CAPACITY;
    // oldest first
    size_t start = 0;
    if (ringCount_ == SH_HISTORY_CAPACITY) {
        start = ringWrite_; // oldest is next write position when full
    } else {
        start = 0;
    }
    // But if not full, oldest is 0..ringCount_-1; if we always write sequentially from 0, yes.
    // General: oldest index = (ringWrite_ + SH_HISTORY_CAPACITY - ringCount_) % SH_HISTORY_CAPACITY
    start = (ringWrite_ + SH_HISTORY_CAPACITY - ringCount_) % SH_HISTORY_CAPACITY;
    for (size_t i = 0; i < n; ++i) {
        dest[i] = ring_[(start + i) % SH_HISTORY_CAPACITY];
    }
    return n;
}

const char* StatisticsHistory::metricIdString(MetricId id) {
    switch (id) {
        case MID_OVERVIEW_TREASURY: return "overview.treasury";
        case MID_OVERVIEW_WALLETS_TOTAL: return "overview.wallets_total";
        case MID_OVERVIEW_MONEY_SUPPLY: return "overview.money_supply";
        case MID_OVERVIEW_INVESTMENT_POOL: return "overview.investment_pool";
        case MID_OVERVIEW_ENTERPRISE_CAPITAL_TOTAL: return "overview.enterprise_capital_total";
        case MID_OVERVIEW_ENTERPRISE_COUNT: return "overview.enterprise_count";
        case MID_FLOW_WAGES: return "flow.wages";
        case MID_FLOW_PENSIONS: return "flow.pensions";
        case MID_FLOW_WELFARE: return "flow.welfare";
        case MID_FLOW_STATE_STORE_SALES: return "flow.state_store_sales";
        case MID_FLOW_WHOLESALE_B2B: return "flow.wholesale_b2b";
        case MID_FLOW_VAT: return "flow.vat";
        case MID_FLOW_PROFIT_TAX: return "flow.profit_tax";
        case MID_FLOW_PROPERTY_TAX: return "flow.property_tax";
        case MID_FLOW_NET_TREASURY: return "flow.net_treasury";
        case MID_FLOW_UNEXPLAINED: return "flow.unexplained";
        case MID_PRIVATE_SALES: return "private.sales";
        case MID_PRIVATE_WAGES: return "private.wages";
        case MID_PRIVATE_TAXES: return "private.taxes";
        case MID_PRIVATE_POOL_INFLOW: return "private.pool_inflow";
        case MID_PRIVATE_ENTERPRISE_COUNT: return "private.enterprise_count";
        case MID_PRIVATE_EVICTIONS: return "private.evictions";
        case MID_PRIVATE_RENT_MONTHLY: return "private.rent_monthly";
        case MID_PRICE_FOOD: return "price.food";
        case MID_PRICE_MEAT: return "price.meat";
        case MID_PRICE_CLOTHES: return "price.clothes";
        case MID_PRICE_ELECTRONICS: return "price.electronics";
        case MID_CPI_DAILY_BASKET: return "price.cpi_daily_basket";
        case MID_MARKET_MULT_FOOD: return "price.market_mult_food";
        case MID_CITIZEN_WALLETS_TOTAL: return "citizen.wallets_total";
        case MID_CITIZEN_COUNT: return "citizen.count";
        case MID_LOGISTICS_FUEL_CONSUMED: return "logistics.fuel_consumed";
        case MID_LOGISTICS_TRIP_COUNT: return "logistics.trip_count";
        case MID_LOGISTICS_TONS_MOVED: return "logistics.tons_moved";
        case MID_LOGISTICS_DRY_RUN_FLAG: return "logistics.dry_run";
        case MID_PRODUCTION_OUTPUT: return "production.output";
        default: return "unknown";
    }
}

} // namespace almanac
