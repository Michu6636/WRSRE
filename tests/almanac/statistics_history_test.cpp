#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <vector>
#include <memory>
#include "statistics_history.h"

using namespace almanac;

static bool near(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) <= eps;
}

// ---- fake source context ----------------------------------------------------
struct Ctx {
    // currency
    int activeCurrency = 0; // 0 RUB
    const char* symbol = "RUB";
    double treasury = 100.0;
    double wallets = 50.0;
    int treasuryOk = 1;
    int walletsOk = 1;
    // enterprise
    size_t entCount = 3;
    double entCapital = 30.0;
    int entCountOk = 1;
    int entCapitalOk = 1;
    int entSummaryOk = 1;
    double privSales = 10.0, privWages = 4.0, privTaxes = 1.0, privPool = 0.5;
    int evictionsOk = 1; size_t evictions = 2;
    int rentOk = 1; double rent = 7.0;
    int poolOk = 1; double pool = 5.0;
    // pricing
    int priceOk = 1;
    double food = 2.0, meat = 3.0, clothes = 4.0, elec = 5.0;
    int cpiOk = 1; double cpi = 2.15;
    double cpiRentSeen = -1.0;
    int multOk = 1; double mult = 1.2;
    int daysPerMonth = 30;
    int periodOk = 1;
    // flow
    int flowOk = 1;
    double wages=1,w2=0,w3=0,store=2,wholesale=3,vat=0.5,profit=0.6,prop=0.7,net=8,unexpl=0.1;
    // logistics
    int logOk = 1;
    double fuel=9.0, trips=4.0, tons=12.0;
    int dryRun = 1;
    int dryRunOk = 1;
    // non-finite injection
    int injectNonFinite = 0;
};

static Ctx G;

// currency callbacks
static int curActive(void* u){ (void)u; return G.activeCurrency; }
static const char* curSym(void* u){ (void)u; return G.symbol; }
static int curTreasury(void* u, int c, double* o){ (void)u;(void)c; if(!G.treasuryOk) return 0; if(G.injectNonFinite){*o=std::numeric_limits<double>::quiet_NaN();return 1;} *o=G.treasury; return 1; }
static int curWallets(void* u, double* o){ (void)u; if(!G.walletsOk) return 0; *o=G.wallets; return 1; }

// clock
static int clkDay(void* u){ (void)u; return 5; }
static int clkMonth(void* u){ (void)u; return 3; }
static int clkYear(void* u){ (void)u; return 2026; }
static int clkDaysPerMonth(void* u){ (void)u; return G.daysPerMonth; }

// enterprise
static int entCount(void* u, size_t* o){ (void)u; if(!G.entCountOk) return 0; *o=G.entCount; return 1; }
static int entCapital(void* u, double* o){ (void)u; if(!G.entCapitalOk) return 0; if(G.injectNonFinite){*o=std::numeric_limits<double>::infinity();return 1;} *o=G.entCapital; return 1; }
static int entSummary(void* u, double* s, double* w, double* t, double* p){ (void)u; if(!G.entSummaryOk) return 0; *s=G.privSales;*w=G.privWages;*t=G.privTaxes;*p=G.privPool; return 1; }
static int entPool(void* u, double* o){ (void)u; if(!G.poolOk) return 0; *o=G.pool; return 1; }
static int entEvict(void* u, size_t* o){ (void)u; if(!G.evictionsOk) return 0; *o=G.evictions; return 1; }
static int entRent(void* u, double* o){ (void)u; if(!G.rentOk) return 0; *o=G.rent; return 1; }

// pricing
static int prFood(void* u, int c, double* o){ (void)u;(void)c; if(!G.priceOk) return 0; *o=G.food; return 1; }
static int prMeat(void* u, int c, double* o){ (void)u;(void)c; if(!G.priceOk) return 0; *o=G.meat; return 1; }
static int prClothes(void* u, int c, double* o){ (void)u;(void)c; if(!G.priceOk) return 0; *o=G.clothes; return 1; }
static int prElec(void* u, int c, double* o){ (void)u;(void)c; if(!G.priceOk) return 0; *o=G.elec; return 1; }
static int prCpi(void* u, double r, double* o){ (void)u; G.cpiRentSeen=r; if(!G.cpiOk) return 0; *o=G.cpi; return 1; }
static int prMult(void* u, double* o){ (void)u; if(!G.multOk) return 0; *o=G.mult; return 1; }

static int metricPeriod(void* u, MetricId id, SimDate* start, SimDate* end){
    (void)u; (void)id;
    if(!G.periodOk) return 0;
    *start = SimDate{31,2,2026};
    *end = SimDate{59,2,2026};
    return 1;
}

// flow
static int fWages(void* u, double* o){ (void)u; if(!G.flowOk) return 0; *o=G.wages; return 1; }
static int fPens(void* u, double* o){ (void)u; if(!G.flowOk) return 0; *o=G.w2; return 1; }
static int fWelf(void* u, double* o){ (void)u; if(!G.flowOk) return 0; *o=G.w3; return 1; }
static int fStore(void* u, double* o){ (void)u; if(!G.flowOk) return 0; *o=G.store; return 1; }
static int fWholesale(void* u, double* o){ (void)u; if(!G.flowOk) return 0; *o=G.wholesale; return 1; }
static int fVat(void* u, double* o){ (void)u; if(!G.flowOk) return 0; *o=G.vat; return 1; }
static int fProfit(void* u, double* o){ (void)u; if(!G.flowOk) return 0; *o=G.profit; return 1; }
static int fProp(void* u, double* o){ (void)u; if(!G.flowOk) return 0; *o=G.prop; return 1; }
static int fNet(void* u, double* o){ (void)u; if(!G.flowOk) return 0; *o=G.net; return 1; }
static int fUnexpl(void* u, double* o){ (void)u; if(!G.flowOk) return 0; *o=G.unexpl; return 1; }

// logistics
static int lFuel(void* u, double* o){ (void)u; if(!G.logOk) return 0; *o=G.fuel; return 1; }
static int lTrips(void* u, double* o){ (void)u; if(!G.logOk) return 0; *o=G.trips; return 1; }
static int lTons(void* u, double* o){ (void)u; if(!G.logOk) return 0; *o=G.tons; return 1; }
static int lDry(void* u, int* o){ (void)u; if(!G.dryRunOk) return 0; *o=G.dryRun; return 1; }

static Sources makeSources() {
    Sources s{};
    s.clock.getDay = clkDay; s.clock.getMonth = clkMonth; s.clock.getYear = clkYear;
    s.clock.getDaysPerMonth = clkDaysPerMonth;
    s.periods.getMetricPeriod = metricPeriod;
    s.currency.getActiveCurrency = curActive; s.currency.getSymbol = curSym;
    s.currency.getTreasury = curTreasury; s.currency.getTotalWallets = curWallets;
    s.enterprise.getCount = entCount; s.enterprise.getCapitalSum = entCapital;
    s.enterprise.getMonthlySummary = entSummary; s.enterprise.getPool = entPool;
    s.enterprise.getMonthlyEvictions = entEvict; s.enterprise.getRentMonthly = entRent;
    s.pricing.getFoodPrice = prFood; s.pricing.getMeatPrice = prMeat;
    s.pricing.getClothesPrice = prClothes; s.pricing.getElectronicsPrice = prElec;
    s.pricing.getCpiBasket = prCpi; s.pricing.getMarketMultFood = prMult;
    s.flow.getWages = fWages; s.flow.getPensions = fPens; s.flow.getWelfare = fWelf;
    s.flow.getStateStoreSales = fStore; s.flow.getWholesale = fWholesale;
    s.flow.getVat = fVat; s.flow.getProfitTax = fProfit; s.flow.getPropertyTax = fProp;
    s.flow.getNetTreasury = fNet; s.flow.getUnexplained = fUnexpl;
    s.logistics.getFuelConsumed = lFuel; s.logistics.getTripCount = lTrips;
    s.logistics.getTonsMoved = lTons; s.logistics.isDryRun = lDry;
    // wire userData all null (globals used)
    return s;
}

static Metric* findMetric(Snapshot& snap, MetricId id) {
    for (size_t i = 0; i < snap.metricCount; ++i) {
        if (snap.metrics[i].id == id) return &snap.metrics[i];
    }
    return nullptr;
}

int main() {
    // (1) initial/unavailable state with no sources
    {
        auto h = std::make_unique<StatisticsHistory>();
        Snapshot s = h->getSnapshot();
        assert(s.metricCount == 0);
        assert(s.coherent == 1);
        assert(h->historyCount() == 0);
    }

    // (2) coherent snapshot with sources
    {
        G = Ctx{};
        auto h = std::make_unique<StatisticsHistory>();
        h->setSources(makeSources());
        SimDate d{5,3,2026};
        bool ok = h->update(d);
        assert(ok);
        Snapshot s = h->getSnapshot();
        assert(s.asOf.year == 2026);
        assert(s.currency == 0);
        // treasury metric present and valid
        Metric* m = findMetric(s, MID_OVERVIEW_TREASURY);
        assert(m != nullptr);
        assert(m->available == 1);
        assert(m->hasValue == 1);
        assert(near(m->value, 100.0));
        assert(m->valueClass == VC_ACTUAL);
        assert(m->kind == MK_STOCK);
        // money supply sums
        Metric* ms = findMetric(s, MID_OVERVIEW_MONEY_SUPPLY);
        assert(ms && ms->available == 1);
        assert(near(ms->value, 100.0+50.0+30.0+5.0));
        Metric* cpi = findMetric(s, MID_CPI_DAILY_BASKET);
        assert(cpi && cpi->available == 1);
        assert(near(G.cpiRentSeen, 7.0 / 30.0));
        // logistics fuel is dry-run, not actual
        Metric* lf = findMetric(s, MID_LOGISTICS_FUEL_CONSUMED);
        assert(lf && lf->available == 1);
        assert(lf->valueClass == VC_DRY_RUN);
    }

    // (3) missing optional sources -> unavailable, no crash
    {
        G = Ctx{};
        G.entSummaryOk = 0; G.priceOk = 0; G.logOk = 0; G.flowOk = 0;
        G.dryRunOk = 0; G.rentOk = 0; G.periodOk = 0;
        auto h = std::make_unique<StatisticsHistory>();
        Sources s = makeSources();
        // remove optional sources entirely
        s.enterprise.getMonthlySummary = nullptr;
        s.pricing.getFoodPrice = nullptr;
        s.logistics.getFuelConsumed = nullptr;
        s.flow.getWages = nullptr;
        h->setSources(s);
        h->update({5,3,2026});
        Snapshot snap = h->getSnapshot();
        Metric* ps = findMetric(snap, MID_PRIVATE_SALES);
        assert(ps && ps->available == 0);
        Metric* pf = findMetric(snap, MID_PRICE_FOOD);
        assert(pf && pf->available == 0);
        Metric* lf = findMetric(snap, MID_LOGISTICS_FUEL_CONSUMED);
        assert(lf && lf->available == 0);
        Metric* fw = findMetric(snap, MID_FLOW_WAGES);
        assert(fw && fw->available == 0);
        assert(!fw->periodStart.valid() && !fw->periodEnd.valid());
        Metric* dry = findMetric(snap, MID_LOGISTICS_DRY_RUN_FLAG);
        assert(dry && dry->available == 0 && dry->hasValue == 0);
        Metric* cpi = findMetric(snap, MID_CPI_DAILY_BASKET);
        assert(cpi && cpi->available == 0);
        // citizen count always unavailable (documented)
        Metric* cc = findMetric(snap, MID_CITIZEN_COUNT);
        assert(cc && cc->available == 0);
    }

    // (4) non-finite source values -> metric unavailable
    {
        G = Ctx{};
        G.injectNonFinite = 1;
        auto h = std::make_unique<StatisticsHistory>();
        h->setSources(makeSources());
        h->update({5,3,2026});
        Snapshot snap = h->getSnapshot();
        Metric* tr = findMetric(snap, MID_OVERVIEW_TREASURY);
        assert(tr && tr->available == 0);
        Metric* ms = findMetric(snap, MID_OVERVIEW_MONEY_SUPPLY);
        assert(ms && ms->available == 0); // enterprise capital non-finite
    }

    // (5) currency & unit separation
    {
        G = Ctx{};
        G.activeCurrency = 1; G.symbol = "USD";
        G.treasury = 10.0;
        auto h = std::make_unique<StatisticsHistory>();
        h->setSources(makeSources());
        h->update({5,3,2026});
        Snapshot snap = h->getSnapshot();
        assert(snap.currency == 1);
        Metric* tr = findMetric(snap, MID_OVERVIEW_TREASURY);
        assert(tr && tr->currency == 1);
        assert(std::strcmp(tr->unit, "USD") == 0);
        // count metric has currency -1
        Metric* ec = findMetric(snap, MID_OVERVIEW_ENTERPRISE_COUNT);
        assert(ec && ec->currency == -1);
        // price unit contains USD/ton
        Metric* pf = findMetric(snap, MID_PRICE_FOOD);
        assert(pf && std::strstr(pf->unit, "USD") != nullptr);
    }

    // (6) actual/dry-run separation: money supply doesn't include dry-run logistics
    {
        G = Ctx{};
        auto h = std::make_unique<StatisticsHistory>();
        h->setSources(makeSources());
        h->update({5,3,2026});
        Snapshot snap = h->getSnapshot();
        Metric* ms = findMetric(snap, MID_OVERVIEW_MONEY_SUPPLY);
        // logistics fuel not part of money supply
        assert(ms && near(ms->value, 100.0+50.0+30.0+5.0));
        // dry-run metric class distinct
        Metric* lf = findMetric(snap, MID_LOGISTICS_FUEL_CONSUMED);
        assert(lf && lf->valueClass == VC_DRY_RUN);
        assert(lf->valueClass != VC_ACTUAL);
    }

    // (7) stock vs flow period handling
    {
        G = Ctx{};
        auto h = std::make_unique<StatisticsHistory>();
        h->setSources(makeSources());
        h->update({5,3,2026});
        Snapshot snap = h->getSnapshot();
        Metric* tr = findMetric(snap, MID_OVERVIEW_TREASURY);
        assert(tr && tr->kind == MK_STOCK);
        assert(tr->periodStart.day == 5 && tr->periodEnd.day == 5);
        Metric* fw = findMetric(snap, MID_FLOW_WAGES);
        assert(fw && fw->kind == MK_FLOW);
        assert(fw->periodStart.day == 31 && fw->periodStart.month == 2);
        assert(fw->periodEnd.day == 59 && fw->periodEnd.month == 2);
    }

    // (8) bounded history wraparound + deterministic ordering
    {
        G = Ctx{};
        auto h = std::make_unique<StatisticsHistory>();
        h->setSources(makeSources());
        for (int i = 0; i < 130; ++i) {
            SimDate d{1,1,2020 + (i/12)};
            // vary month
            int m = (i % 12) + 1;
            d.month = m;
            d.day = 1;
            h->update(d);
            h->recordHistory(d);
        }
        assert(h->historyCount() == SH_HISTORY_CAPACITY);
        std::vector<Snapshot> buf(SH_HISTORY_CAPACITY);
        size_t n = h->readHistory(buf.data(), SH_HISTORY_CAPACITY);
        assert(n == SH_HISTORY_CAPACITY);
        // deterministic oldest->newest: years non-decreasing overall
        for (size_t i = 1; i < n; ++i) {
            // sequence increasing
            assert(buf[i].sequence >= buf[i-1].sequence);
        }
        // repeated reads stable
        std::vector<Snapshot> buf2(SH_HISTORY_CAPACITY);
        h->readHistory(buf2.data(), SH_HISTORY_CAPACITY);
        for (size_t i = 0; i < n; ++i) assert(buf[i].sequence == buf2[i].sequence);
    }

    // (9) reset and backward-time handling
    {
        G = Ctx{};
        auto h = std::make_unique<StatisticsHistory>();
        h->setSources(makeSources());
        h->update({5,3,2026});
        h->recordHistory({5,3,2026});
        assert(h->historyCount() == 1);
        // backward time
        bool ok = h->update({1,1,2025});
        assert(!ok);
        assert(h->historyCount() == 0);
        // currency change resets
        G.activeCurrency = 1; G.symbol = "USD";
        h->update({5,3,2026});
        // history empty after currency reset? update on currency change clears history
        // (already empty)
        assert(h->historyCount() == 0);
        // explicit reset
        h->reset(RR_SAVE_LOAD);
        Snapshot s = h->getSnapshot();
        assert(s.metricCount == 0 || findMetric(s, MID_OVERVIEW_TREASURY) == nullptr || findMetric(s,MID_OVERVIEW_TREASURY)->available == 0);
    }

    // (10) repeated reads without growth
    {
        G = Ctx{};
        auto h = std::make_unique<StatisticsHistory>();
        h->setSources(makeSources());
        h->update({5,3,2026});
        size_t before = h->historyCount();
        for (int i = 0; i < 100; ++i) { Snapshot s = h->getSnapshot(); (void)s; }
        assert(h->historyCount() == before);
    }

    // (11) coherent snapshot copy not live
    {
        G = Ctx{};
        auto h = std::make_unique<StatisticsHistory>();
        h->setSources(makeSources());
        h->update({5,3,2026});
        Snapshot a = h->getSnapshot();
        G.treasury = 999.0;
        h->update({6,3,2026});
        // old snapshot unchanged
        Metric* ma = findMetric(a, MID_OVERVIEW_TREASURY);
        assert(ma && near(ma->value, 100.0));
    }

    printf("ALL TESTS PASSED\n");
    return 0;
}
