#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "almanac_composition.h"
#include "almanac_page_providers.h"
#include "currency_api.h"
#include "enterprise_base_api.h"
#include "fiscal_policy.h"
#include "resource_pricing_api.h"
#include "republic_bank_api.h"
#include "sim_clock_api.h"
#include "taxes_api.h"

using namespace almanac;

#define CHECK(x) do { if (!(x)) { \
    std::fprintf(stderr, "FAIL %d: %s\n", __LINE__, #x); \
    std::exit(1); \
} } while (0)

enum ServiceBit {
    OMIT_CLOCK = 1 << 0,
    OMIT_CURRENCY = 1 << 1,
    OMIT_ENTERPRISE = 1 << 2,
    OMIT_PRICING = 1 << 3,
    OMIT_TAXES = 1 << 4,
    OMIT_BANK = 1 << 5
};

struct FakeWorld {
    TsmSimClockApi clock{};
    TsmCurrencyApi currency{};
    TsmEnterpriseBaseApi enterprise{};
    TsmResourcePricingApi pricing{};
    TsmTaxesApi taxes{};
    TsmRepublicBankApi bank{};

    int omitMask = 0;
    int consumeCalls = 0;
    int statReads = 0;
    int clockReads = 0;
    unsigned clockVersion = 0;
    unsigned currencyVersion = 0;
    unsigned enterpriseVersion = 0;
    unsigned pricingVersion = 0;
    unsigned taxesVersion = 0;
    unsigned bankVersion = 0;

    int day = 5;
    int month = 3;
    int year = 2026;
    int daysPerMonth = 30;
    int activeCurrency = 0;
    double treasury = 1000.0;
    double wallets = 200.0;

    std::size_t enterpriseCount = 2;
    double capitals[2] = {30.0, 40.0};
    double pool = 5.0;
    double privateSales = 100.0;
    double privateWages = 40.0;
    double privateTaxes = 10.0;
    double privatePool = 2.0;
    std::size_t evictions = 3;
    double rent = 7.0;
    int buildingTokens[2] = {};

    bool recordsAvailable = true;
    int foodToken = 0;
    int meatToken = 0;
    int clothesToken = 0;
    int electronicsToken = 0;
    double foodPrice = 2.0;
    double meatPrice = 3.0;
    double clothesPrice = 4.0;
    double electronicsPrice = 5.0;
    double marketFood = 1.2;
    double cpiRentSeen = -1.0;

    int vatEnabled = 1;
    int profitEnabled = 0;
    int propertyEnabled = 1;
    float vatRate = 0.20f;
    float profitRate = 0.15f;
    float propertyAmount = 250.0f;
    double bankRate = 0.06;
};

static FakeWorld G;

int fakeDay() { ++G.statReads; ++G.clockReads; return G.day; }
int fakeMonth() { ++G.statReads; ++G.clockReads; return G.month; }
int fakeYear() { ++G.statReads; ++G.clockReads; return G.year; }
int fakeDaysPerMonth() { ++G.statReads; ++G.clockReads; return G.daysPerMonth; }

int fakeActiveCurrency() { ++G.statReads; return G.activeCurrency; }
const char* fakeCurrencySymbol() {
    ++G.statReads;
    return G.activeCurrency == 1 ? "USD" : "RUB";
}
double fakeTreasury(int currency) {
    ++G.statReads;
    return currency == G.activeCurrency ? G.treasury : G.treasury * 0.5;
}
double fakeWallets() { ++G.statReads; return G.wallets; }

std::size_t fakeEnterpriseCount() { ++G.statReads; return G.enterpriseCount; }
void* fakeEnterpriseByIndex(std::size_t index) {
    ++G.statReads;
    if (index >= G.enterpriseCount || index >= 2) return nullptr;
    return &G.buildingTokens[index];
}
double fakeCompanyCapital(void* building) {
    ++G.statReads;
    for (std::size_t i = 0; i < 2; ++i) {
        if (building == &G.buildingTokens[i]) return G.capitals[i];
    }
    return 0.0;
}
double fakePool() { ++G.statReads; return G.pool; }
void fakeMonthlySummary(double* sales, double* wages, double* taxes, double* pool) {
    ++G.statReads;
    if (sales) *sales = G.privateSales;
    if (wages) *wages = G.privateWages;
    if (taxes) *taxes = G.privateTaxes;
    if (pool) *pool = G.privatePool;
}
std::size_t fakeEvictions() { ++G.statReads; return G.evictions; }
double fakeRent() { ++G.statReads; return G.rent; }

void* fakeFood() { ++G.statReads; return G.recordsAvailable ? &G.foodToken : nullptr; }
void* fakeMeat() { ++G.statReads; return G.recordsAvailable ? &G.meatToken : nullptr; }
void* fakeClothes() { ++G.statReads; return G.recordsAvailable ? &G.clothesToken : nullptr; }
void* fakeElectronics() {
    ++G.statReads;
    return G.recordsAvailable ? &G.electronicsToken : nullptr;
}
float fakePrice(void* record, int currency) {
    ++G.statReads;
    (void)currency;
    if (record == &G.foodToken) return static_cast<float>(G.foodPrice);
    if (record == &G.meatToken) return static_cast<float>(G.meatPrice);
    if (record == &G.clothesToken) return static_cast<float>(G.clothesPrice);
    if (record == &G.electronicsToken) return static_cast<float>(G.electronicsPrice);
    return 0.0f;
}
void fakeRecordSupply(void* record, double tons) { (void)record; (void)tons; }
void fakeRecordDemand(void* record, double tons) { (void)record; (void)tons; }
float fakeMarketMultiplier(void* record) {
    ++G.statReads;
    return record == &G.foodToken ? static_cast<float>(G.marketFood) : 1.0f;
}
void fakeMarketBalance(void* record, double* supply, double* demand, float* multiplier) {
    (void)record;
    if (supply) *supply = 0.0;
    if (demand) *demand = 0.0;
    if (multiplier) *multiplier = 1.0f;
}
float fakeCpi(float dailyRent) {
    ++G.statReads;
    G.cpiRentSeen = dailyRent;
    return static_cast<float>(2.0 + dailyRent);
}

float fakeVatRate() { return G.vatEnabled ? G.vatRate : 0.0f; }
float fakeProfitRate() { return G.profitEnabled ? G.profitRate : 0.0f; }
int fakeTaxEnabled(int kind) {
    if (kind == TSM_TAX_VAT) return G.vatEnabled;
    if (kind == TSM_TAX_PROFIT) return G.profitEnabled;
    if (kind == TSM_TAX_PROPERTY) return G.propertyEnabled;
    return 0;
}
int fakeSetTaxEnabled(int kind, int enabled) {
    if (kind == TSM_TAX_VAT) G.vatEnabled = enabled ? 1 : 0;
    else if (kind == TSM_TAX_PROFIT) G.profitEnabled = enabled ? 1 : 0;
    else if (kind == TSM_TAX_PROPERTY) G.propertyEnabled = enabled ? 1 : 0;
    else return 0;
    return 1;
}
int fakeSetTaxRate(int kind, float rate) {
    if (kind == TSM_TAX_PROPERTY) {
        if (rate < 0.0f) return 0;
        G.propertyAmount = rate;
        return 1;
    }
    if (rate < 0.0f || rate > 1.0f) return 0;
    if (kind == TSM_TAX_VAT) G.vatRate = rate;
    else if (kind == TSM_TAX_PROFIT) G.profitRate = rate;
    else return 0;
    return 1;
}
int fakeConfiguredRate(int kind, float* out) {
    if (!out) return 0;
    if (kind == TSM_TAX_VAT) *out = G.vatRate;
    else if (kind == TSM_TAX_PROFIT) *out = G.profitRate;
    else if (kind == TSM_TAX_PROPERTY) *out = G.propertyAmount;
    else return 0;
    return 1;
}
int fakeTaxStep() { return 2; }
float fakePropertyMax() { return 500.0f; }
int fakeTaxPolicy(TsmTaxPolicy* out) {
    if (!out || out->structSize < sizeof(TsmTaxPolicy)) return 0;
    out->vatEnabled = G.vatEnabled;
    out->profitEnabled = G.profitEnabled;
    out->propertyEnabled = G.propertyEnabled;
    out->vatRate = G.vatRate;
    out->profitRate = G.profitRate;
    out->propertyAmount = G.propertyAmount;
    out->sliderStepPct = 2;
    out->propertySliderMax = 500.0f;
    return 1;
}
double fakeBankRate() { return G.bankRate; }
void fakeSetBankRate(double rate) { G.bankRate = rate; }

const void* fakeConsume(const char* service, unsigned version) {
    ++G.consumeCalls;
    if (std::strcmp(service, TSM_SERVICE_SIM_CLOCK) == 0) {
        G.clockVersion = version;
        return (G.omitMask & OMIT_CLOCK) ? nullptr : &G.clock;
    }
    if (std::strcmp(service, TSM_SERVICE_CURRENCY) == 0) {
        G.currencyVersion = version;
        return (G.omitMask & OMIT_CURRENCY) ? nullptr : &G.currency;
    }
    if (std::strcmp(service, TSM_SERVICE_ENTERPRISE_BASE) == 0) {
        G.enterpriseVersion = version;
        return (G.omitMask & OMIT_ENTERPRISE) ? nullptr : &G.enterprise;
    }
    if (std::strcmp(service, TSM_SERVICE_RESOURCE_PRICING) == 0) {
        G.pricingVersion = version;
        return (G.omitMask & OMIT_PRICING) ? nullptr : &G.pricing;
    }
    if (std::strcmp(service, TSM_SERVICE_TAXES) == 0) {
        G.taxesVersion = version;
        return (G.omitMask & OMIT_TAXES) ? nullptr : &G.taxes;
    }
    if (std::strcmp(service, TSM_SERVICE_REPUBLIC_BANK) == 0) {
        G.bankVersion = version;
        return (G.omitMask & OMIT_BANK) ? nullptr : &G.bank;
    }
    return nullptr;
}

TsmHost makeHost() {
    TsmHost host{};
    host.structSize = sizeof(TsmHost);
    host.consume = fakeConsume;
    return host;
}

void resetFake() {
    G = FakeWorld{};

    G.clock.structSize = sizeof(TsmSimClockApi);
    G.clock.getDay = fakeDay;
    G.clock.getMonth = fakeMonth;
    G.clock.getYear = fakeYear;
    G.clock.getDaysPerMonth = fakeDaysPerMonth;

    G.currency.structSize = sizeof(TsmCurrencyApi);
    G.currency.getTreasuryBalance = fakeTreasury;
    G.currency.getTotalWalletBalance = fakeWallets;
    G.currency.getActiveCurrency = fakeActiveCurrency;
    G.currency.getCurrencySymbol = fakeCurrencySymbol;

    G.enterprise.structSize = sizeof(TsmEnterpriseBaseApi);
    G.enterprise.getEnterpriseCount = fakeEnterpriseCount;
    G.enterprise.getEnterpriseByIndex = fakeEnterpriseByIndex;
    G.enterprise.getCompanyCapital = fakeCompanyCapital;
    G.enterprise.getPrivateInvestmentPool = fakePool;
    G.enterprise.getMonthlyEconomySummary = fakeMonthlySummary;
    G.enterprise.getMonthlyEvictionCount = fakeEvictions;
    G.enterprise.getResidentialRentMonthly = fakeRent;

    G.pricing.structSize = sizeof(TsmResourcePricingApi);
    G.pricing.getFood = fakeFood;
    G.pricing.getMeat = fakeMeat;
    G.pricing.getClothes = fakeClothes;
    G.pricing.getEletronics = fakeElectronics;
    G.pricing.getPrice = fakePrice;
    G.pricing.recordSupply = fakeRecordSupply;
    G.pricing.recordDemand = fakeRecordDemand;
    G.pricing.getMarketMultiplier = fakeMarketMultiplier;
    G.pricing.getMarketBalance = fakeMarketBalance;
    G.pricing.getCpiDailyBasket = fakeCpi;

    G.taxes.structSize = sizeof(TsmTaxesApi);
    G.taxes.getVatRate = fakeVatRate;
    G.taxes.getProfitTaxRate = fakeProfitRate;
    G.taxes.isTaxEnabled = fakeTaxEnabled;
    G.taxes.setTaxRate = fakeSetTaxRate;
    G.taxes.setTaxEnabled = fakeSetTaxEnabled;
    G.taxes.getTaxConfiguredRate = fakeConfiguredRate;
    G.taxes.getTaxSliderStepPct = fakeTaxStep;
    G.taxes.getTaxPropertySliderMax = fakePropertyMax;
    G.taxes.getTaxPolicy = fakeTaxPolicy;

    G.bank.structSize = sizeof(TsmRepublicBankApi);
    G.bank.getAnnualInterestRate = fakeBankRate;
    G.bank.setAnnualInterestRate = fakeSetBankRate;
}

AlmanacHostSurface makeSurface() {
    AlmanacHostSurface host{};
    host.capabilities = AlmanacHostCapability_Visibility |
                        AlmanacHostCapability_SelectionValidation |
                        AlmanacHostCapability_Geometry |
                        AlmanacHostCapability_Dpi |
                        AlmanacHostCapability_Clipping |
                        AlmanacHostCapability_Scrolling |
                        AlmanacHostCapability_PointerInput |
                        AlmanacHostCapability_FrameIdentity |
                        AlmanacHostCapability_LifecycleInvalidation |
                        AlmanacHostCapability_FlowLayout;
    host.visible = true;
    host.selected = true;
    host.geometry.dpi = 2.0f;
    host.geometry.origin_x = 10.0f;
    host.geometry.origin_y = 20.0f;
    host.geometry.extent_width = 500.0f;
    host.geometry.extent_height = 1000.0f;
    host.clip.enabled = true;
    host.clip.x = 10.0f;
    host.clip.y = 20.0f;
    host.clip.width = 500.0f;
    host.clip.height = 1000.0f;
    host.scroll.can_scroll_y = true;
    host.scroll.max_offset_y = 1000.0f;
    host.pointer.x = 15.0f;
    host.pointer.y = 25.0f;
    host.pointer.in_bounds = true;
    host.pointer.action = AlmanacPointerAction_Move;
    host.pointer.frame_id = 1;
    host.frame.frame_number = 1;
    host.frame.generation = 1;
    host.lifecycle.generation = 1;
    return host;
}

const AlmanacPageSnapshot* populatePage(AlmanacComposition& composition,
                                        AlmanacPageId page,
                                        AlmanacHostSurface& host) {
    CHECK(composition.shell() != nullptr);
    CHECK(almanac_shell_navigate(composition.shell(), page));
    CHECK(almanac_shell_populate(composition.shell(), &host));
    const AlmanacPageSnapshot* snapshot =
        almanac_shell_page_snapshot(composition.shell(), page);
    CHECK(snapshot != nullptr);
    return snapshot;
}

int findRow(const AlmanacPageSnapshot* snapshot, const char* label) {
    if (!snapshot) return -1;
    for (std::size_t i = 0; i < snapshot->rowCount; ++i) {
        if (std::strcmp(snapshot->rows[i].label, label) == 0) return static_cast<int>(i);
    }
    return -1;
}

int findRowContaining(const AlmanacPageSnapshot* snapshot, const char* text) {
    if (!snapshot || !text) return -1;
    for (std::size_t i = 0; i < snapshot->rowCount; ++i) {
        if (std::strstr(snapshot->rows[i].label, text)) return static_cast<int>(i);
    }
    return -1;
}

causation_ledger::EventNode makeEvent(std::uint64_t id, std::uint64_t parent,
                                      causation_ledger::ValueClass valueClass,
                                      causation_ledger::EvidenceStatus evidence) {
    causation_ledger::EventNode node{};
    node.eventId = id;
    node.parentEventId = parent;
    node.sequence = id;
    node.simTime = static_cast<double>(id * 10);
    node.amount = static_cast<double>(id);
    node.valueClass = valueClass;
    node.evidence = evidence;
    node.valid = true;
    causation_ledger::CopyStr(node.sourcePlugin, sizeof(node.sourcePlugin), "test");
    causation_ledger::CopyStr(node.sourceKind, sizeof(node.sourceKind), "event");
    causation_ledger::CopyStr(node.subjectKey, sizeof(node.subjectKey), "subject");
    causation_ledger::CopyStr(node.unit, sizeof(node.unit), "RUB");
    return node;
}

void testFullComposition() {
    resetFake();
    TsmHost host = makeHost();
    AlmanacComposition composition(&host);
    CHECK(composition.ready());
    CHECK(composition.shell() != nullptr);
    CHECK(almanac_shell_provider_count(composition.shell()) == 7);
    CHECK(G.consumeCalls == 6);
    CHECK(G.clockVersion == TSM_SIM_CLOCK_VERSION);
    CHECK(G.currencyVersion == TSM_CURRENCY_VERSION);
    CHECK(G.enterpriseVersion == TSM_ENTERPRISE_BASE_VERSION);
    CHECK(G.pricingVersion == TSM_RESOURCE_PRICING_VERSION);
    CHECK(G.taxesVersion == TSM_TAXES_VERSION);
    CHECK(G.bankVersion == TSM_REPUBLIC_BANK_VERSION);

    AlmanacHostSurface surface = makeSurface();
    G.statReads = 0;
    const AlmanacPageSnapshot* snapshot =
        populatePage(composition, ALMANAC_PAGE_OVERVIEW, surface);
    CHECK(snapshot->rowCount == 0);
    CHECK(!snapshot->available);
    CHECK(std::strstr(snapshot->emptyState, "not been updated") != nullptr);
    CHECK(G.statReads == 0);

    CHECK(composition.updateStatistics());
    CHECK(G.statReads > 0);
    snapshot = populatePage(composition, ALMANAC_PAGE_OVERVIEW, surface);
    CHECK(snapshot->rowCount == 6);
    CHECK(snapshot->available);
    int row = findRowContaining(snapshot, "overview.treasury");
    CHECK(row >= 0);
    CHECK(std::strcmp(snapshot->rows[row].value, "1000.00") == 0);
    CHECK(snapshot->rows[row].valueClass == ALMANAC_VALUE_ACTUAL);
    row = findRowContaining(snapshot, "overview.money_supply");
    CHECK(row >= 0);
    CHECK(std::strcmp(snapshot->rows[row].value, "1275.00") == 0);
    CHECK(std::fabs(G.cpiRentSeen - 7.0 / 30.0) < 1e-6);

    const int readsAfterUpdate = G.statReads;
    const AlmanacPageId pages[5] = {
        ALMANAC_PAGE_OVERVIEW,
        ALMANAC_PAGE_MONEY_FLOW,
        ALMANAC_PAGE_PRIVATE_ECONOMY,
        ALMANAC_PAGE_PRICES_CITIZENS,
        ALMANAC_PAGE_LOGISTICS_PRODUCTION
    };
    const std::size_t expectedRows[5] = {6, 10, 7, 8, 5};
    for (std::size_t i = 0; i < 5; ++i) {
        snapshot = populatePage(composition, pages[i], surface);
        CHECK(snapshot->populated);
        CHECK(snapshot->rowCount == expectedRows[i]);
        if (pages[i] == ALMANAC_PAGE_MONEY_FLOW) {
            CHECK(!snapshot->available);
            row = findRowContaining(snapshot, "flow.wages");
            CHECK(row >= 0);
            CHECK(std::strcmp(snapshot->rows[row].value, "unavailable") == 0);
        }
        if (pages[i] == ALMANAC_PAGE_LOGISTICS_PRODUCTION) {
            row = findRowContaining(snapshot, "logistics.fuel_consumed");
            CHECK(row >= 0);
            CHECK(snapshot->rows[row].valueClass == ALMANAC_VALUE_DRY_RUN);
            CHECK(!snapshot->rows[row].available);
            row = findRowContaining(snapshot, "logistics.dry_run");
            CHECK(row >= 0);
            CHECK(snapshot->rows[row].available);
            CHECK(snapshot->rows[row].valueClass == ALMANAC_VALUE_ACTUAL);
            CHECK(std::strcmp(snapshot->rows[row].value, "1") == 0);
        }
    }
    CHECK(G.statReads == readsAfterUpdate);

    snapshot = populatePage(composition, ALMANAC_PAGE_FISCAL_POLICY, surface);
    row = findRow(snapshot, "VAT rate");
    CHECK(row >= 0);
    CHECK(std::strcmp(snapshot->rows[row].value, "20%") == 0);
    row = findRow(snapshot, "Bank interest rate");
    CHECK(row >= 0);
    CHECK(std::strcmp(snapshot->rows[row].value, "6%") == 0);

    const int vatBefore = G.vatEnabled;
    row = findRow(snapshot, "VAT toggle");
    CHECK(row >= 0);
    surface.scroll.offset_y = static_cast<float>(row) * 70.0f;
    surface.pointer.action = AlmanacPointerAction_Down;
    surface.pointer.y = 25.0f;
    CHECK(almanac_shell_handle_pointer(composition.shell(), &surface));
    CHECK(G.vatEnabled != vatBefore);
    CHECK(fakeSetTaxEnabled(TSM_TAX_VAT, vatBefore));
    surface.pointer.action = AlmanacPointerAction_Move;
    surface.scroll.offset_y = 0.0f;

    CHECK(composition.recordStatisticsHistory());
    CHECK(composition.statisticsHistoryCount() == 1);
    G.activeCurrency = 1;
    CHECK(composition.updateStatistics());
    snapshot = populatePage(composition, ALMANAC_PAGE_OVERVIEW, surface);
    row = findRowContaining(snapshot, "overview.treasury");
    CHECK(row >= 0);
    CHECK(snapshot->rows[row].currency == 1);
    CHECK(std::strcmp(snapshot->rows[row].unit, "USD") == 0);
    snapshot = populatePage(composition, ALMANAC_PAGE_FISCAL_POLICY, surface);
    row = findRow(snapshot, "Property tax amount");
    CHECK(row >= 0);
    CHECK(std::strcmp(snapshot->rows[row].unit, "USD") == 0);
    CHECK(std::strcmp(snapshot->rows[row].value, "250.00") == 0);
}

void testCausationAndLifecycle() {
    resetFake();
    TsmHost host = makeHost();
    AlmanacComposition composition(&host);
    CHECK(composition.ready());

    using causation_ledger::EvidenceStatus;
    using causation_ledger::ValueClass;
    CHECK(composition.addCausationNode(makeEvent(1, 0, ValueClass::Actual,
                                                 EvidenceStatus::Verified)));
    CHECK(composition.addCausationNode(makeEvent(2, 1, ValueClass::DryRun,
                                                 EvidenceStatus::Supplied)));
    CHECK(composition.addCausationNode(makeEvent(3, 999, ValueClass::Estimated,
                                                 EvidenceStatus::Unresolved)));
    causation_ledger::CausalEdge edge{};
    edge.edgeId = 1;
    edge.fromEventId = 1;
    edge.toEventId = 2;
    edge.kind = causation_ledger::EdgeKind::Causes;
    edge.evidence = EvidenceStatus::Verified;
    edge.valid = true;
    CHECK(composition.addCausationEdge(edge));
    edge.edgeId = 2;
    edge.fromEventId = 2;
    edge.toEventId = 3;
    edge.evidence = EvidenceStatus::Unresolved;
    CHECK(composition.addCausationEdge(edge));

    AlmanacHostSurface surface = makeSurface();
    const AlmanacPageSnapshot* snapshot =
        populatePage(composition, ALMANAC_PAGE_CAUSATION, surface);
    CHECK(snapshot->available);
    CHECK(snapshot->rowCount == 9);
    int row = findRow(snapshot, "Events shown");
    CHECK(row >= 0);
    CHECK(std::strcmp(snapshot->rows[row].value, "3 / 3") == 0);
    row = findRow(snapshot, "Edges shown");
    CHECK(row >= 0);
    CHECK(std::strcmp(snapshot->rows[row].value, "2 / 2") == 0);
    row = findRow(snapshot, "Missing parents");
    CHECK(row >= 0);
    CHECK(std::strcmp(snapshot->rows[row].value, "1") == 0);
    CHECK(findRowContaining(snapshot, "[DRY_RUN]") >= 0);
    CHECK(findRowContaining(snapshot, "[ESTIMATED]") >= 0);
    CHECK(findRow(snapshot, "edge 1 causes [verified]") >= 0);
    CHECK(findRow(snapshot, "edge 2 causes [unresolved]") >= 0);

    CHECK(composition.recordStatisticsHistory());
    CHECK(composition.statisticsHistoryCount() == 1);
    G.day = 4;
    CHECK(composition.updateStatistics());
    CHECK(composition.statisticsHistoryCount() == 0);
    snapshot = populatePage(composition, ALMANAC_PAGE_CAUSATION, surface);
    CHECK(snapshot->rowCount == 0);
    CHECK(std::strcmp(snapshot->emptyState, "No causation events") == 0);

    G.day = 6;
    CHECK(composition.recordStatisticsHistory());
    CHECK(composition.statisticsHistoryCount() == 1);
    composition.reset(RR_SAVE_LOAD);
    CHECK(composition.statisticsHistoryCount() == 0);

    G.day = 7;
    CHECK(composition.updateStatistics());
    snapshot = populatePage(composition, ALMANAC_PAGE_OVERVIEW, surface);
    CHECK(snapshot->rowCount == 6);
    G.day = -1;
    CHECK(!composition.updateStatistics());
    snapshot = populatePage(composition, ALMANAC_PAGE_OVERVIEW, surface);
    CHECK(snapshot->rowCount == 0);

    composition.shutdown();
    CHECK(!composition.ready());
    CHECK(composition.shell() == nullptr);
    CHECK(!composition.updateStatistics());
    CHECK(!composition.addCausationNode(makeEvent(4, 0, ValueClass::Actual,
                                                  EvidenceStatus::Supplied)));
    composition.shutdown();
}

void testCausationDisplayBudget() {
    resetFake();
    TsmHost host = makeHost();
    AlmanacComposition composition(&host);
    for (std::uint64_t id = 1; id <= 70; ++id) {
        CHECK(composition.addCausationNode(makeEvent(
            id, 0, causation_ledger::ValueClass::Actual,
            causation_ledger::EvidenceStatus::Supplied)));
    }
    causation_ledger::CausalEdge edge{};
    edge.edgeId = 1;
    edge.fromEventId = 1;
    edge.toEventId = 2;
    edge.evidence = causation_ledger::EvidenceStatus::Verified;
    edge.valid = true;
    CHECK(composition.addCausationEdge(edge));

    AlmanacHostSurface surface = makeSurface();
    const AlmanacPageSnapshot* snapshot =
        populatePage(composition, ALMANAC_PAGE_CAUSATION, surface);
    CHECK(snapshot->rowCount == ALMANAC_MAX_ROWS);
    CHECK(snapshot->available);
    CHECK(snapshot->stale);
    int row = findRow(snapshot, "Causation snapshot");
    CHECK(row >= 0);
    CHECK(std::strcmp(snapshot->rows[row].value, "incomplete: display budget") == 0);
    row = findRow(snapshot, "Events shown");
    CHECK(row >= 0);
    CHECK(std::strcmp(snapshot->rows[row].value, "60 / 70") == 0);
    row = findRow(snapshot, "Edges shown");
    CHECK(row >= 0);
    CHECK(std::strcmp(snapshot->rows[row].value, "0 / 1") == 0);
    CHECK(findRow(snapshot, "Missing parents") >= 0);
}

void testUnknownValueAndRelationMetadata() {
    resetFake();
    TsmHost host = makeHost();
    AlmanacComposition composition(&host);
    CHECK(composition.addCausationNode(makeEvent(
        1, 0, causation_ledger::ValueClass::Actual,
        causation_ledger::EvidenceStatus::Verified)));
    CHECK(composition.addCausationNode(makeEvent(
        2, 0, static_cast<causation_ledger::ValueClass>(99),
        causation_ledger::EvidenceStatus::Supplied)));
    causation_ledger::CausalEdge edge{};
    edge.edgeId = 1;
    edge.fromEventId = 1;
    edge.toEventId = 2;
    edge.kind = static_cast<causation_ledger::EdgeKind>(99);
    edge.evidence = causation_ledger::EvidenceStatus::Verified;
    edge.valid = true;
    CHECK(composition.addCausationEdge(edge));

    AlmanacHostSurface surface = makeSurface();
    const AlmanacPageSnapshot* snapshot =
        populatePage(composition, ALMANAC_PAGE_CAUSATION, surface);
    int row = findRowContaining(snapshot, "[UNKNOWN]");
    CHECK(row >= 0);
    CHECK(snapshot->rows[row].valueClass == ALMANAC_VALUE_INVALID);
    CHECK(findRow(snapshot, "edge 1 unknown [verified]") >= 0);
}

void testMissingAndMalformedServices() {
    {
        resetFake();
        G.omitMask = OMIT_CURRENCY | OMIT_ENTERPRISE | OMIT_PRICING |
                     OMIT_TAXES | OMIT_BANK;
        TsmHost host = makeHost();
        AlmanacComposition composition(&host);
        CHECK(composition.ready());
        CHECK(almanac_shell_provider_count(composition.shell()) == 7);
        CHECK(composition.updateStatistics());
        AlmanacHostSurface surface = makeSurface();
        const AlmanacPageSnapshot* snapshot =
            populatePage(composition, ALMANAC_PAGE_OVERVIEW, surface);
        CHECK(snapshot->rowCount == 6);
        CHECK(!snapshot->available);
        for (std::size_t i = 0; i < snapshot->rowCount; ++i) {
            CHECK(!snapshot->rows[i].available);
            CHECK(std::strcmp(snapshot->rows[i].value, "unavailable") == 0);
        }
        snapshot = populatePage(composition, ALMANAC_PAGE_FISCAL_POLICY, surface);
        int row = findRow(snapshot, "VAT toggle");
        CHECK(row >= 0);
        CHECK(!snapshot->rows[row].interactive);
        CHECK(!snapshot->rows[row].enabled);
        row = findRow(snapshot, "Bank interest rate");
        CHECK(row >= 0);
        CHECK(std::strcmp(snapshot->rows[row].value, "unavailable") == 0);
    }

    {
        resetFake();
        G.omitMask = OMIT_CURRENCY;
        TsmHost host = makeHost();
        AlmanacComposition composition(&host);
        AlmanacHostSurface surface = makeSurface();
        const AlmanacPageSnapshot* snapshot =
            populatePage(composition, ALMANAC_PAGE_FISCAL_POLICY, surface);
        int row = findRow(snapshot, "Property tax amount");
        CHECK(row >= 0);
        CHECK(snapshot->rows[row].available);
        CHECK(snapshot->rows[row].currency == -1);
        CHECK(std::strcmp(snapshot->rows[row].unit, "") == 0);
        CHECK(std::strcmp(snapshot->rows[row].value, "250.00") == 0);

        CHECK(composition.updateStatistics());
        snapshot = populatePage(composition, ALMANAC_PAGE_OVERVIEW, surface);
        row = findRowContaining(snapshot, "overview.treasury");
        CHECK(row >= 0 && !snapshot->rows[row].available);
        row = findRowContaining(snapshot, "overview.enterprise_capital_total");
        CHECK(row >= 0 && !snapshot->rows[row].available);
        row = findRowContaining(snapshot, "overview.enterprise_count");
        CHECK(row >= 0 && snapshot->rows[row].available);
        snapshot = populatePage(composition, ALMANAC_PAGE_PRICES_CITIZENS, surface);
        row = findRowContaining(snapshot, "price.food");
        CHECK(row >= 0 && !snapshot->rows[row].available);
        row = findRowContaining(snapshot, "price.market_mult_food");
        CHECK(row >= 0 && snapshot->rows[row].available);
    }

    {
        resetFake();
        G.recordsAvailable = false;
        TsmHost host = makeHost();
        AlmanacComposition composition(&host);
        CHECK(composition.updateStatistics());
        AlmanacHostSurface surface = makeSurface();
        const AlmanacPageSnapshot* snapshot =
            populatePage(composition, ALMANAC_PAGE_PRICES_CITIZENS, surface);
        int row = findRowContaining(snapshot, "price.food");
        CHECK(row >= 0 && !snapshot->rows[row].available);
        row = findRowContaining(snapshot, "price.cpi_daily_basket");
        CHECK(row >= 0 && !snapshot->rows[row].available);
        row = findRowContaining(snapshot, "price.market_mult_food");
        CHECK(row >= 0 && !snapshot->rows[row].available);
        CHECK(G.cpiRentSeen < 0.0);
    }

    {
        resetFake();
        TsmHost host = makeHost();
        host.structSize = offsetof(TsmHost, consume);
        AlmanacComposition composition(&host);
        CHECK(composition.ready());
        CHECK(G.consumeCalls == 0);
        CHECK(!composition.updateStatistics());
    }

    {
        AlmanacComposition composition(nullptr);
        CHECK(composition.ready());
        CHECK(almanac_shell_provider_count(composition.shell()) == 7);
        CHECK(!composition.updateStatistics());
    }

    {
        resetFake();
        G.clock.structSize = offsetof(TsmSimClockApi, getYear);
        TsmHost host = makeHost();
        AlmanacComposition composition(&host);
        G.statReads = 0;
        CHECK(!composition.updateStatistics());
        CHECK(G.clockReads == 0);
    }
}

int main() {
    testFullComposition();
    testCausationAndLifecycle();
    testCausationDisplayBudget();
    testUnknownValueAndRelationMetadata();
    testMissingAndMalformedServices();
    std::printf("ALL TESTS PASSED\n");
    return 0;
}
