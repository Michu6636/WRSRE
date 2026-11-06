#include "almanac_composition.h"

#include "almanac_page_providers.h"
#include "currency_api.h"
#include "enterprise_base_api.h"
#include "fiscal_policy.h"
#include "resource_pricing_api.h"
#include "republic_bank_api.h"
#include "sim_clock_api.h"
#include "taxes_api.h"

#include <cmath>
#include <cstddef>
#include <new>

namespace {

struct Services {
    const TsmSimClockApi* clock = nullptr;
    const TsmCurrencyApi* currency = nullptr;
    const TsmEnterpriseBaseApi* enterprise = nullptr;
    const TsmResourcePricingApi* pricing = nullptr;
    const TsmTaxesApi* taxes = nullptr;
    const TsmRepublicBankApi* bank = nullptr;
};

Services consumeServices(const TsmHost* host) {
    Services services;
    if (!host ||
        host->structSize < offsetof(TsmHost, consume) + sizeof(host->consume) ||
        !host->consume) {
        return services;
    }
    services.clock = static_cast<const TsmSimClockApi*>(
        host->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION));
    services.currency = static_cast<const TsmCurrencyApi*>(
        host->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION));
    services.enterprise = static_cast<const TsmEnterpriseBaseApi*>(
        host->consume(TSM_SERVICE_ENTERPRISE_BASE, TSM_ENTERPRISE_BASE_VERSION));
    services.pricing = static_cast<const TsmResourcePricingApi*>(
        host->consume(TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION));
    services.taxes = static_cast<const TsmTaxesApi*>(
        host->consume(TSM_SERVICE_TAXES, TSM_TAXES_VERSION));
    services.bank = static_cast<const TsmRepublicBankApi*>(
        host->consume(TSM_SERVICE_REPUBLIC_BANK, TSM_REPUBLIC_BANK_VERSION));
    return services;
}

bool clockHasDate(const TsmSimClockApi* api) {
    return api &&
           api->structSize >= offsetof(TsmSimClockApi, getYear) + sizeof(api->getYear) &&
           api->getDay && api->getMonth && api->getYear;
}

bool clockHasDaysPerMonth(const TsmSimClockApi* api) {
    return api &&
           api->structSize >= offsetof(TsmSimClockApi, getDaysPerMonth) +
                                  sizeof(api->getDaysPerMonth) &&
           api->getDaysPerMonth;
}

int clockDay(void* userData) {
    const Services* services = static_cast<const Services*>(userData);
    return clockHasDate(services->clock) ? services->clock->getDay() : -1;
}

int clockMonth(void* userData) {
    const Services* services = static_cast<const Services*>(userData);
    return clockHasDate(services->clock) ? services->clock->getMonth() : -1;
}

int clockYear(void* userData) {
    const Services* services = static_cast<const Services*>(userData);
    return clockHasDate(services->clock) ? services->clock->getYear() : -1;
}

int clockDaysPerMonth(void* userData) {
    const Services* services = static_cast<const Services*>(userData);
    return clockHasDaysPerMonth(services->clock)
               ? services->clock->getDaysPerMonth()
               : 0;
}

int currencyActive(void* userData) {
    const Services* services = static_cast<const Services*>(userData);
    return TsmCurrencyActive(services->currency);
}

const char* currencySymbol(void* userData) {
    const Services* services = static_cast<const Services*>(userData);
    return TsmCurrencySymbol(services->currency);
}

int currencyTreasury(void* userData, int currency, double* out) {
    const Services* services = static_cast<const Services*>(userData);
    const TsmCurrencyApi* api = services->currency;
    if (!out || !api || !TsmCurrencyHasActive(api) ||
        api->structSize < offsetof(TsmCurrencyApi, getTreasuryBalance) +
                              sizeof(api->getTreasuryBalance) ||
        !api->getTreasuryBalance) return 0;
    const double value = api->getTreasuryBalance(currency);
    if (!std::isfinite(value)) return 0;
    *out = value;
    return 1;
}

int currencyWallets(void* userData, double* out) {
    const Services* services = static_cast<const Services*>(userData);
    const TsmCurrencyApi* api = services->currency;
    if (!out || !api || !TsmCurrencyHasActive(api) ||
        api->structSize < offsetof(TsmCurrencyApi, getTotalWalletBalance) +
                              sizeof(api->getTotalWalletBalance) ||
        !api->getTotalWalletBalance) return 0;
    const double value = api->getTotalWalletBalance();
    if (!std::isfinite(value)) return 0;
    *out = value;
    return 1;
}

int enterpriseCount(void* userData, std::size_t* out) {
    const Services* services = static_cast<const Services*>(userData);
    const TsmEnterpriseBaseApi* api = services->enterprise;
    if (!out || !api ||
        api->structSize < offsetof(TsmEnterpriseBaseApi, getEnterpriseCount) +
                              sizeof(api->getEnterpriseCount) ||
        !api->getEnterpriseCount) return 0;
    *out = api->getEnterpriseCount();
    return 1;
}

int enterpriseCapitalSum(void* userData, double* out) {
    const Services* services = static_cast<const Services*>(userData);
    const TsmEnterpriseBaseApi* api = services->enterprise;
    if (!out || !api || !TsmCurrencyHasActive(services->currency) ||
        api->structSize < offsetof(TsmEnterpriseBaseApi, getCompanyCapital) +
                              sizeof(api->getCompanyCapital) ||
        !api->getEnterpriseCount || !api->getEnterpriseByIndex ||
        !api->getCompanyCapital) return 0;

    const std::size_t count = api->getEnterpriseCount();
    double sum = 0.0;
    for (std::size_t i = 0; i < count; ++i) {
        void* building = api->getEnterpriseByIndex(i);
        if (!building) return 0;
        const double capital = api->getCompanyCapital(building);
        if (!std::isfinite(capital)) return 0;
        sum += capital;
        if (!std::isfinite(sum)) return 0;
    }
    // The public API has no aggregate-lock snapshot. Reject an enumeration
    // that changed size instead of presenting a known-partial sum as complete.
    if (api->getEnterpriseCount() != count) return 0;
    *out = sum;
    return 1;
}

int enterprisePool(void* userData, double* out) {
    const Services* services = static_cast<const Services*>(userData);
    const TsmEnterpriseBaseApi* api = services->enterprise;
    if (!out || !api || !TsmCurrencyHasActive(services->currency) ||
        api->structSize < offsetof(TsmEnterpriseBaseApi, getPrivateInvestmentPool) +
                              sizeof(api->getPrivateInvestmentPool) ||
        !api->getPrivateInvestmentPool) return 0;
    const double value = api->getPrivateInvestmentPool();
    if (!std::isfinite(value)) return 0;
    *out = value;
    return 1;
}

int enterpriseSummary(void* userData, double* sales, double* wages,
                      double* taxes, double* pool) {
    const Services* services = static_cast<const Services*>(userData);
    const TsmEnterpriseBaseApi* api = services->enterprise;
    if (!sales || !wages || !taxes || !pool || !api ||
        !TsmCurrencyHasActive(services->currency) ||
        api->structSize < offsetof(TsmEnterpriseBaseApi, getMonthlyEconomySummary) +
                              sizeof(api->getMonthlyEconomySummary) ||
        !api->getMonthlyEconomySummary) return 0;
    double s = 0.0, w = 0.0, t = 0.0, p = 0.0;
    api->getMonthlyEconomySummary(&s, &w, &t, &p);
    if (!std::isfinite(s) || !std::isfinite(w) ||
        !std::isfinite(t) || !std::isfinite(p)) return 0;
    *sales = s;
    *wages = w;
    *taxes = t;
    *pool = p;
    return 1;
}

int enterpriseEvictions(void* userData, std::size_t* out) {
    const Services* services = static_cast<const Services*>(userData);
    const TsmEnterpriseBaseApi* api = services->enterprise;
    if (!out || !api ||
        api->structSize < offsetof(TsmEnterpriseBaseApi, getMonthlyEvictionCount) +
                              sizeof(api->getMonthlyEvictionCount) ||
        !api->getMonthlyEvictionCount) return 0;
    *out = api->getMonthlyEvictionCount();
    return 1;
}

int enterpriseRent(void* userData, double* out) {
    const Services* services = static_cast<const Services*>(userData);
    const TsmEnterpriseBaseApi* api = services->enterprise;
    if (!out || !api || !TsmCurrencyHasActive(services->currency) ||
        api->structSize < offsetof(TsmEnterpriseBaseApi, getResidentialRentMonthly) +
                              sizeof(api->getResidentialRentMonthly) ||
        !api->getResidentialRentMonthly) return 0;
    const double value = api->getResidentialRentMonthly();
    if (!std::isfinite(value) || value < 0.0) return 0;
    *out = value;
    return 1;
}

void* pricingRecord(const TsmResourcePricingApi* api, int which) {
    if (!api ||
        api->structSize < offsetof(TsmResourcePricingApi, getPrice) + sizeof(api->getPrice) ||
        !api->getPrice) return nullptr;
    switch (which) {
        case 0: return api->getFood ? api->getFood() : nullptr;
        case 1: return api->getMeat ? api->getMeat() : nullptr;
        case 2: return api->getClothes ? api->getClothes() : nullptr;
        case 3: return api->getEletronics ? api->getEletronics() : nullptr;
        default: return nullptr;
    }
}

int pricingPrice(void* userData, int which, int currency, double* out) {
    const Services* services = static_cast<const Services*>(userData);
    const TsmResourcePricingApi* api = services->pricing;
    if (!out || !api || !TsmCurrencyHasActive(services->currency)) return 0;
    void* record = pricingRecord(api, which);
    if (!record) return 0;
    const float value = api->getPrice(record, currency);
    if (!std::isfinite(value) || value <= 0.0f) return 0;
    *out = value;
    return 1;
}

int pricingFood(void* userData, int currency, double* out) {
    return pricingPrice(userData, 0, currency, out);
}
int pricingMeat(void* userData, int currency, double* out) {
    return pricingPrice(userData, 1, currency, out);
}
int pricingClothes(void* userData, int currency, double* out) {
    return pricingPrice(userData, 2, currency, out);
}
int pricingElectronics(void* userData, int currency, double* out) {
    return pricingPrice(userData, 3, currency, out);
}

int pricingCpi(void* userData, double dailyRent, double* out) {
    const Services* services = static_cast<const Services*>(userData);
    const TsmResourcePricingApi* api = services->pricing;
    if (!out || !TsmPricingHasCpi(api) ||
        !TsmCurrencyHasActive(services->currency) ||
        !std::isfinite(dailyRent) || dailyRent < 0.0) return 0;
    // The pricing API has configured fallbacks even without a world. Do not
    // present those fallback numbers as live ACTUAL metrics before records exist.
    if (!pricingRecord(api, 0) || !pricingRecord(api, 1) || !pricingRecord(api, 2)) return 0;
    const float value = api->getCpiDailyBasket(static_cast<float>(dailyRent));
    if (!std::isfinite(value) || value <= 0.0f) return 0;
    *out = value;
    return 1;
}

int pricingMarketFood(void* userData, double* out) {
    const Services* services = static_cast<const Services*>(userData);
    const TsmResourcePricingApi* api = services->pricing;
    if (!out || !TsmPricingHasMarket(api)) return 0;
    void* food = pricingRecord(api, 0);
    if (!food) return 0;
    const float value = api->getMarketMultiplier(food);
    if (!std::isfinite(value) || value <= 0.0f) return 0;
    *out = value;
    return 1;
}

almanac::Sources makeSources(Services& services) {
    almanac::Sources sources{};
    sources.clock.userData = &services;
    sources.currency.userData = &services;
    sources.enterprise.userData = &services;
    sources.pricing.userData = &services;

    if (services.clock) {
        sources.clock.getDay = clockDay;
        sources.clock.getMonth = clockMonth;
        sources.clock.getYear = clockYear;
        if (clockHasDaysPerMonth(services.clock)) {
            sources.clock.getDaysPerMonth = clockDaysPerMonth;
        }
    }
    if (services.currency) {
        if (TsmCurrencyHasActive(services.currency)) {
            sources.currency.getActiveCurrency = currencyActive;
            sources.currency.getSymbol = currencySymbol;
        }
        sources.currency.getTreasury = currencyTreasury;
        sources.currency.getTotalWallets = currencyWallets;
    }
    if (services.enterprise) {
        sources.enterprise.getCount = enterpriseCount;
        sources.enterprise.getCapitalSum = enterpriseCapitalSum;
        sources.enterprise.getPool = enterprisePool;
        sources.enterprise.getMonthlySummary = enterpriseSummary;
        sources.enterprise.getMonthlyEvictions = enterpriseEvictions;
        sources.enterprise.getRentMonthly = enterpriseRent;
    }
    if (services.pricing) {
        sources.pricing.getFoodPrice = pricingFood;
        sources.pricing.getMeatPrice = pricingMeat;
        sources.pricing.getClothesPrice = pricingClothes;
        sources.pricing.getElectronicsPrice = pricingElectronics;
        sources.pricing.getCpiBasket = pricingCpi;
        sources.pricing.getMarketMultFood = pricingMarketFood;
    }

    // No public closed-period treasury-flow, logistics-metric, production, or
    // exact period-source contract is consumed here. Those metrics retain
    // their explicit unavailable state instead of using partial observations.
    return sources;
}

bool readCurrentDate(const Services& services, almanac::SimDate* out) {
    if (!out || !clockHasDate(services.clock)) return false;
    const almanac::SimDate date{
        services.clock->getDay(),
        services.clock->getMonth(),
        services.clock->getYear()
    };
    if (!date.valid() || date.day > 364) return false;
    *out = date;
    return true;
}

int compareDates(const almanac::SimDate& a, const almanac::SimDate& b) {
    if (a.year != b.year) return a.year < b.year ? -1 : 1;
    if (a.month != b.month) return a.month < b.month ? -1 : 1;
    if (a.day != b.day) return a.day < b.day ? -1 : 1;
    return 0;
}

} // namespace

namespace almanac {

struct AlmanacComposition::Impl {
    Services services;
    AlmanacShell* shell = nullptr;
    fiscal_policy::Adapter fiscal;
    StatisticsHistory statistics;
    causation_ledger::Ledger causation;
    std::mutex causationMutex;
    StatisticsPageContext statisticsContext;
    CausationPageContext causationContext;
    int statisticsHandles[5] = {};
    int causationHandle = 0;
    bool readyFlag = false;
    bool haveValidDate = false;
    SimDate lastDate{};

    explicit Impl(const TsmHost* host)
        : services(consumeServices(host)),
          shell(almanac_shell_create()),
          fiscal(shell, services.taxes, services.bank,
                 TsmCurrencyHasActive(services.currency)
                     ? TsmCurrencyActive(services.currency) : -1),
          statistics(),
          causation(),
          causationMutex(),
          statisticsContext{&statistics, true},
          causationContext{&causation, &causationMutex, true} {
        statistics.setSources(makeSources(services));
        syncCurrency();
        if (!shell || !fiscal.registered()) {
            shutdown();
            return;
        }

        static const AlmanacPageId statisticsPages[5] = {
            ALMANAC_PAGE_OVERVIEW,
            ALMANAC_PAGE_MONEY_FLOW,
            ALMANAC_PAGE_PRIVATE_ECONOMY,
            ALMANAC_PAGE_PRICES_CITIZENS,
            ALMANAC_PAGE_LOGISTICS_PRODUCTION
        };
        for (std::size_t i = 0; i < 5; ++i) {
            statisticsHandles[i] = almanac_shell_register_provider(
                shell, statisticsPages[i], populateStatisticsPage, &statisticsContext);
            if (statisticsHandles[i] <= 0) {
                shutdown();
                return;
            }
        }
        causationHandle = almanac_shell_register_provider(
            shell, ALMANAC_PAGE_CAUSATION, populateCausationPage, &causationContext);
        if (causationHandle <= 0) {
            shutdown();
            return;
        }
        readyFlag = true;
    }

    ~Impl() {
        shutdown();
    }

    void syncCurrency() {
        fiscal.context().currency = TsmCurrencyHasActive(services.currency)
            ? TsmCurrencyActive(services.currency) : -1;
    }

    void resetDerived(ResetReason reason) {
        statistics.reset(reason);
        {
            std::lock_guard<std::mutex> lock(causationMutex);
            causation.resetAll();
        }
        haveValidDate = false;
        lastDate = SimDate{};
    }

    void shutdown() {
        readyFlag = false;
        if (!shell) return;
        statisticsContext.alive = false;
        causationContext.alive = false;
        for (std::size_t i = 0; i < 5; ++i) {
            if (statisticsHandles[i] > 0) {
                almanac_shell_unregister_provider(shell, statisticsHandles[i]);
                statisticsHandles[i] = 0;
            }
        }
        if (causationHandle > 0) {
            almanac_shell_unregister_provider(shell, causationHandle);
            causationHandle = 0;
        }
        fiscal.unregister();
        almanac_shell_destroy(shell);
        shell = nullptr;
        resetDerived(RR_MANUAL);
    }

};

AlmanacComposition::AlmanacComposition(const TsmHost* host)
    : impl_(new (std::nothrow) Impl(host)) {
}

AlmanacComposition::~AlmanacComposition() {
    delete impl_;
}

bool AlmanacComposition::ready() const {
    return impl_ && impl_->readyFlag && impl_->shell;
}

AlmanacShell* AlmanacComposition::shell() const {
    return impl_ ? impl_->shell : nullptr;
}

bool AlmanacComposition::updateStatistics() {
    if (!ready()) return false;
    impl_->syncCurrency();

    SimDate current;
    if (!readCurrentDate(impl_->services, &current)) {
        if (impl_->haveValidDate) impl_->resetDerived(RR_WORLD_CHANGE);
        return false;
    }

    if (impl_->haveValidDate && compareDates(current, impl_->lastDate) < 0) {
        std::lock_guard<std::mutex> lock(impl_->causationMutex);
        impl_->causation.resetAll();
    }
    // StatisticsHistory::update returns false on backward time but still
    // rebuilds the current snapshot after clearing derived history.
    impl_->statistics.update(current);
    impl_->lastDate = current;
    impl_->haveValidDate = true;
    return true;
}

bool AlmanacComposition::recordStatisticsHistory() {
    if (!updateStatistics()) return false;
    impl_->statistics.recordHistory(impl_->lastDate);
    return true;
}

std::size_t AlmanacComposition::statisticsHistoryCount() const {
    return impl_ ? impl_->statistics.historyCount() : 0;
}

void AlmanacComposition::reset(ResetReason reason) {
    if (impl_) impl_->resetDerived(reason);
}

bool AlmanacComposition::addCausationNode(const causation_ledger::EventNode& node) {
    if (!ready()) return false;
    std::lock_guard<std::mutex> lock(impl_->causationMutex);
    return impl_->causation.addNode(node);
}

bool AlmanacComposition::addCausationEdge(const causation_ledger::CausalEdge& edge) {
    if (!ready()) return false;
    std::lock_guard<std::mutex> lock(impl_->causationMutex);
    return impl_->causation.addEdge(edge);
}

bool AlmanacComposition::observeCausationTime(double simTime) {
    if (!ready()) return false;
    std::lock_guard<std::mutex> lock(impl_->causationMutex);
    return impl_->causation.observeTime(simTime);
}

void AlmanacComposition::shutdown() {
    if (impl_) impl_->shutdown();
}

} // namespace almanac
