// Built by run_tax_policy_tests.py against extracted production bodies.
#include "almanac_finance_view.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <initializer_list>
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #x); std::exit(1); } } while (0)
static float g_vatRate = .2f, g_profitTaxRate = .3f, g_propertyTaxFlat = 12.5f;
static int g_vatEnabled = 0, g_profitEnabled = 1, g_propertyEnabled = 1, g_propertyLandValue = 1;
static int g_lock = 0, enters = 0, leaves = 0, legacyReads = 0;
static bool changeOnUnlock = false;
static void EnterCriticalSection(int* lock) { CHECK(!*lock); *lock = 1; ++enters; }
static void LeaveCriticalSection(int* lock) {
    CHECK(*lock); *lock = 0; ++leaves;
    if (changeOnUnlock) g_vatRate = .9f;
}
static float GetVatRateImpl() { ++legacyReads; return 0; }
static float GetProfitTaxRateImpl() { ++legacyReads; return 0; }
static void RecordVatAccrualImpl(void*, double) { CHECK(false); }
static void SettleNowImpl() { CHECK(false); }
static int IsTaxEnabledImpl(int) { ++legacyReads; return 0; }
static int SetTaxRateImpl(int, float) { CHECK(false); return 0; }
static int SetTaxEnabledImpl(int, int) { CHECK(false); return 0; }
static const TsmTaxesApi* g_taxes = nullptr;
struct Host {
    const TsmTaxesApi* provided[4]{};
    unsigned onlyVersion = 0, calls = 0;
    void provide(const char* name, unsigned version, const void* api) {
        CHECK(std::strcmp(name, TSM_SERVICE_TAXES) == 0 && version > 0 && version < 4);
        provided[version] = static_cast<const TsmTaxesApi*>(api);
    }
    const void* consume(const char*, unsigned version) {
        ++calls;
        return version == onlyVersion ? provided[version] : nullptr;
    }
};
static Host host;
static Host* H = &host;
#include "tax_policy_provider.inc"

static void Provider() {
    struct Guarded { TsmTaxPolicySnapshot data; unsigned guard; } out{};
    std::memset(&out, 0x5a, sizeof(out));
    const Guarded before = out;
    CHECK(!GetPolicySnapshotImpl(nullptr, sizeof(out.data)));
    for (unsigned n = 0; n < sizeof(out.data); ++n)
        CHECK(!GetPolicySnapshotImpl(&out.data, n));
    CHECK(std::memcmp(&out, &before, sizeof(out)) == 0 && enters == 0);
    CHECK(!GetPolicySnapshotImpl(&out.data, sizeof(out.data))); // not started
    CHECK(std::memcmp(&out, &before, sizeof(out)) == 0);
    CHECK(enters == leaves);
    g_policySnapshotReady = 1;
    enters = leaves = 0;
    changeOnUnlock = true;
    CHECK(GetPolicySnapshotImpl(&out.data, sizeof(out)));
    changeOnUnlock = false;
    CHECK(enters == 1 && leaves == 1 && !g_lock);
    CHECK(out.data.structSize == sizeof(out.data));
    CHECK(out.data.configured[TSM_TAX_VAT] == .2f && g_vatRate == .9f);
    CHECK(out.data.enabled[TSM_TAX_VAT] == 0); // configured survives suspension
    CHECK(out.data.configured[TSM_TAX_PROPERTY] == 12.5f);
    CHECK(out.data.propertyLandValue == 1 && out.guard == before.guard);
    g_propertyLandValue = 0; g_propertyEnabled = -7;
    CHECK(GetPolicySnapshotImpl(&out.data, sizeof(out.data)));
    CHECK(out.data.propertyLandValue == 0 && out.data.enabled[TSM_TAX_PROPERTY] == 1);
    g_vatRate = .2f; g_propertyEnabled = 1; g_propertyLandValue = 1;
    CHECK(legacyReads == 0);
}

// Original prefix shapes: appended v3 member must not move/resize old fields.
struct LegacyV1 {
    unsigned structSize;
    float (*getVatRate)();
    float (*getProfitTaxRate)();
    void (*recordVatAccrual)(void*, double);
    void (*settleNow)();
};
struct LegacyV2 {
    LegacyV1 prefix;
    int (*isTaxEnabled)(int);
    int (*setTaxRate)(int, float);
    int (*setTaxEnabled)(int, int);
};
static_assert(sizeof(LegacyV1) == offsetof(TsmTaxesApi, isTaxEnabled), "v1 ABI");
static_assert(sizeof(LegacyV2) == offsetof(TsmTaxesApi, getPolicySnapshot), "v2 ABI");
static_assert(offsetof(LegacyV1, settleNow) == offsetof(TsmTaxesApi, settleNow), "v1 offsets");
static_assert(offsetof(LegacyV2, setTaxEnabled) == offsetof(TsmTaxesApi, setTaxEnabled), "v2 offsets");

static void PublicationAndConsumers() {
    PublishExtracted();
    CHECK(host.provided[1]->structSize == offsetof(TsmTaxesApi, isTaxEnabled));
    CHECK(host.provided[2]->structSize == offsetof(TsmTaxesApi, getPolicySnapshot));
    CHECK(host.provided[3]->structSize == sizeof(TsmTaxesApi));
    CHECK(!TsmTaxesHasPolicySnapshot(host.provided[1]));
    CHECK(!TsmTaxesHasPolicySnapshot(host.provided[2]));
    CHECK(TsmTaxesHasPolicySnapshot(host.provided[3]));
    CHECK(!TsmTaxesHasPolicy(host.provided[1]));
    CHECK(TsmTaxesHasPolicy(host.provided[2]));
    CHECK(host.provided[1]->getVatRate == kApi.getVatRate);
    CHECK(host.provided[2]->setTaxRate == kApi.setTaxRate);
    for (auto consumer : {ConsumeEnterpriseExtracted, ConsumeShopsExtracted}) {
        for (unsigned version : {0u, 1u, 2u, 3u}) {
            host.onlyVersion = version; host.calls = 0;
            consumer();
            CHECK(g_taxes == (version ? host.provided[version] : nullptr));
            CHECK(host.calls == (version == 3 ? 1u : version == 2 ? 2u : 3u));
        }
    }
}

static TsmTaxPolicySnapshot response{};
static int responseOk = 1, snapshotCalls = 0;
static int FakeSnapshot(TsmTaxPolicySnapshot* out, unsigned size) {
    CHECK(out && size >= sizeof(*out)); ++snapshotCalls;
    *out = response;
    return responseOk;
}
static int CurrencyCode() { return TSM_CURRENCY_USD; }
static void FinanceConsumer() {
    using namespace almanac;
    const int before = legacyReads;
    auto s = BuildFinanceSnapshot(nullptr, &kApi);
    CHECK(!s.vat.enabled.value && s.vat.configuredFraction.value > .19);
    CHECK(s.vat.effectiveFraction.value == 0 && s.vat.effectiveFraction.Available());
    CHECK(s.profit.effectiveFraction.value == s.profit.configuredFraction.value);
    CHECK(s.property.baseAmountPerEnterprise.value == 12.5);
    CHECK(s.property.rule.value == PropertyTaxRule::AttractivenessScaled);
    CHECK(legacyReads == before);
    TsmCurrencyApi c{}; c.structSize = sizeof(c); c.getActiveCurrency = CurrencyCode;
    auto view = BuildFinanceView(BuildFinanceSnapshot(&c, &kApi));
    CHECK(std::strcmp(view.rows[3].value, "20.0%") == 0);
    CHECK(std::strcmp(view.rows[8].value, "12.50 USD / enterprise") == 0);
    CHECK(std::strstr(view.rows[9].value, "attractiveness"));
    g_propertyLandValue = 0;
    CHECK(std::strcmp(BuildFinanceView(BuildFinanceSnapshot(&c, &kApi)).rows[9].value, "Flat base") == 0);
    CHECK(GetPolicySnapshotImpl(&response, sizeof(response)));
    TsmTaxesApi fake = kApi; fake.getPolicySnapshot = FakeSnapshot;
    for (unsigned n = 0; n <= sizeof(fake); ++n) {
        fake.structSize = n;
        const int calls = snapshotCalls;
        (void)BuildFinanceSnapshot(nullptr, &fake);
        CHECK(snapshotCalls == calls + (n == sizeof(fake) ? 1 : 0));
    }
    fake.structSize = sizeof(fake);
    responseOk = 0;
    const int readsBeforeFailure = legacyReads;
    s = BuildFinanceSnapshot(nullptr, &fake);
    CHECK(legacyReads == readsBeforeFailure);
    CHECK(s.vat.configuredFraction.availability == FinanceAvailability::ReadFailed);
    CHECK(s.property.enabled.availability == FinanceAvailability::ReadFailed);
    responseOk = 1;
    for (int bad = 0; bad < 5; ++bad) {
        CHECK(GetPolicySnapshotImpl(&response, sizeof(response)));
        switch (bad) {
        case 0: response.structSize = 0; break;
        case 1: response.configured[0] = std::numeric_limits<float>::quiet_NaN(); break;
        case 2: response.enabled[1] = 7; break;
        case 3: response.configured[2] = -1; break;
        case 4: response.propertyLandValue = -1; break;
        }
        s = BuildFinanceSnapshot(nullptr, &fake);
        CHECK(s.vat.enabled.availability == FinanceAvailability::InvalidValue);
        CHECK(s.property.baseAmountPerEnterprise.availability == FinanceAvailability::InvalidValue);
    }
}
int main() {
    Provider();
    PublicationAndConsumers();
    FinanceConsumer();
    CHECK(enters == leaves && !g_lock);
    std::puts("tax policy provider/API/finance: all tests passed");
}
