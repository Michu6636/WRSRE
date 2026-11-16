#include "almanac_finance_view.h"
#include <cstdlib>
#include <cstring>
#include <limits>
#include <initializer_list>
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #x); std::exit(1); } } while (0)
using namespace almanac;
using A = FinanceAvailability;
static int code = TSM_CURRENCY_USD, treasuryCalls = 0, rateCalls = 0, statusCalls = 0;
static double balance = -1234.5;
static float vat = 0.2f, profit = 0.15f;
static int enabled[3] = {1, 1, 1};
static int Active() { return code; }
static double Treasury(int c) { CHECK(c == code); ++treasuryCalls; return balance; }
static float Vat() { ++rateCalls; return vat; }
static float Profit() { ++rateCalls; return profit; }
static int Enabled(int kind) { CHECK(kind >= 0 && kind < 3); ++statusCalls; return enabled[kind]; }
static TsmCurrencyApi Currency() {
    TsmCurrencyApi a{}; a.structSize = sizeof(a);
    a.getActiveCurrency = Active; a.getTreasuryBalance = Treasury;
    return a; // Symbol intentionally null: use the validated currency CODE.
}
static TsmTaxesApi Taxes() {
    TsmTaxesApi a{}; a.structSize = sizeof(a);
    a.getVatRate = Vat; a.getProfitTaxRate = Profit; a.isTaxEnabled = Enabled;
    return a; // All mutators null; read-only model must not require them.
}
static void MissingAndNormal() {
    auto s = BuildFinanceSnapshot(nullptr, nullptr);
    CHECK(s.treasury.availability == A::ServiceMissing);
    CHECK(s.vat.enabled.availability == A::ServiceMissing);
    CHECK(s.vat.effectiveFraction.availability == A::ServiceMissing);
    CHECK(s.property.rule.availability == A::ServiceMissing);
    CHECK(std::strstr(BuildFinanceView(s).rows[0].value, "Unavailable"));
    auto c = Currency(); auto t = Taxes();
    s = BuildFinanceSnapshot(&c, &t);
    CHECK(s.activeCurrency.Available() && s.activeCurrency.value == TSM_CURRENCY_USD);
    CHECK(s.treasury.Available() && s.treasury.value == -1234.5);
    CHECK(s.vat.enabled.Available() && s.vat.enabled.value);
    CHECK(s.vat.effectiveFraction.Available());
    CHECK(s.vat.configuredFraction.availability == A::NotExposed);
    CHECK(s.profit.configuredFraction.availability == A::NotExposed);
    CHECK(s.property.baseAmountPerEnterprise.availability == A::NotExposed);
    CHECK(s.property.rule.availability == A::NotExposed);
    const auto v = BuildFinanceView(s);
    CHECK(std::strcmp(v.rows[0].value, "-1234.50 USD") == 0);
    CHECK(std::strcmp(v.rows[2].value, "20.0%") == 0);
    CHECK(std::strcmp(v.rows[5].value, "15.0%") == 0);
    CHECK(std::strstr(v.rows[3].value, "not exposed"));
    CHECK(std::strstr(v.rows[8].label, "per enterprise"));
    CHECK(!std::strchr(v.rows[8].value, '%'));
    enabled[0] = 0; vat = 0.0f; balance = 0; code = TSM_CURRENCY_RUB;
    s = BuildFinanceSnapshot(&c, &t);
    CHECK(s.vat.effectiveFraction.Available() && s.vat.effectiveFraction.value == 0);
    CHECK(!s.vat.enabled.value && s.vat.configuredFraction.availability == A::NotExposed);
    CHECK(std::strcmp(BuildFinanceView(s).rows[0].value, "0.00 RUB") == 0);
    CHECK(std::strcmp(BuildFinanceView(s).rows[1].value, "Suspended") == 0);
    enabled[0] = 1; // Enabled zero still distinct from suspended zero.
    CHECK(BuildFinanceSnapshot(&c, &t).vat.enabled.value);
    vat = 0.2f;
    CHECK(s.vat.effectiveFraction.value == 0); // copied snapshot is not live storage
    enabled[0] = 0; // Deliberately inconsistent sequential reads: do not invent policy.
    CHECK(BuildFinanceSnapshot(&c, &t).vat.effectiveFraction.value > 0.19);
    enabled[0] = 1;
    CHECK(std::strstr(v.note, "cannot distinguish read failure from zero"));
}

static void InvalidValues() {
    auto c = Currency(); auto t = Taxes();
    const int before = treasuryCalls;
    code = 123;
    auto s = BuildFinanceSnapshot(&c, &t);
    CHECK(s.activeCurrency.availability == A::InvalidValue);
    CHECK(!s.treasury.Available() && treasuryCalls == before);
    code = TSM_CURRENCY_USD;
    for (double bad : {std::numeric_limits<double>::infinity(), std::numeric_limits<double>::quiet_NaN()}) {
        balance = bad;
        CHECK(BuildFinanceSnapshot(&c, &t).treasury.availability == A::InvalidValue);
    }
    balance = std::numeric_limits<double>::max();
    CHECK(std::strcmp(BuildFinanceView(BuildFinanceSnapshot(&c, &t)).rows[0].value,
                      "Out of display range") == 0);
    balance = 0;
    for (float bad : {-0.1f, 1.1f, std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN()}) {
        vat = bad;
        CHECK(BuildFinanceSnapshot(&c, &t).vat.effectiveFraction.availability == A::InvalidValue);
    }
    vat = 1;
    CHECK(std::strcmp(BuildFinanceView(BuildFinanceSnapshot(&c, &t)).rows[2].value, "100.0%") == 0);
    vat = 0.2f; enabled[2] = -1;
    CHECK(BuildFinanceSnapshot(&c, &t).property.enabled.availability == A::InvalidValue);
    enabled[2] = 1;
}

static void PrefixesAndNullGetters() {
    auto c = Currency(); auto t = Taxes();
    // Exercise every possible advertised byte size with instrumented callbacks.
    for (unsigned n = 0; n <= sizeof(c); ++n) {
        c.structSize = n;
        const int before = treasuryCalls;
        const auto s = BuildFinanceSnapshot(&c, nullptr);
        const bool hasActive = n >= offsetof(TsmCurrencyApi, getActiveCurrency) + sizeof(c.getActiveCurrency);
        CHECK(s.activeCurrency.Available() == hasActive);
        CHECK(s.treasury.Available() == hasActive);
        CHECK(treasuryCalls == before + (hasActive ? 1 : 0));
    }
    for (unsigned n = 0; n <= sizeof(t); ++n) {
        t.structSize = n;
        const int ratesBefore = rateCalls, statusBefore = statusCalls;
        const auto s = BuildFinanceSnapshot(nullptr, &t);
        const bool v = n >= offsetof(TsmTaxesApi, getVatRate) + sizeof(t.getVatRate);
        const bool p = n >= offsetof(TsmTaxesApi, getProfitTaxRate) + sizeof(t.getProfitTaxRate);
        const bool e = n >= offsetof(TsmTaxesApi, isTaxEnabled) + sizeof(t.isTaxEnabled);
        CHECK(s.vat.effectiveFraction.Available() == v);
        CHECK(s.profit.effectiveFraction.Available() == p);
        CHECK(s.property.enabled.Available() == e);
        CHECK(rateCalls == ratesBefore + (v ? 1 : 0) + (p ? 1 : 0));
        CHECK(statusCalls == statusBefore + (e ? 3 : 0));
    }
    c = Currency(); t = Taxes();
    c.structSize = static_cast<unsigned>(offsetof(TsmCurrencyApi, getActiveCurrency)); // v3 prefix
    t.structSize = static_cast<unsigned>(offsetof(TsmTaxesApi, isTaxEnabled)); // v1 prefix
    auto s = BuildFinanceSnapshot(&c, &t);
    CHECK(!s.activeCurrency.Available() && !s.treasury.Available()); // no invented RUB fallback
    CHECK(s.vat.effectiveFraction.Available() && !s.vat.enabled.Available());
    c = Currency(); c.getActiveCurrency = nullptr;
    const int beforeMissingActive = treasuryCalls;
    CHECK(!BuildFinanceSnapshot(&c, &t).activeCurrency.Available());
    CHECK(treasuryCalls == beforeMissingActive);
    c = Currency(); c.getTreasuryBalance = nullptr;
    t = Taxes(); t.isTaxEnabled = nullptr; t.getVatRate = nullptr;
    s = BuildFinanceSnapshot(&c, &t);
    CHECK(s.activeCurrency.Available() && s.treasury.availability == A::GetterMissing);
    CHECK(s.vat.effectiveFraction.availability == A::GetterMissing);
    CHECK(s.vat.enabled.availability == A::GetterMissing);
    CHECK(s.profit.effectiveFraction.Available());
    // Build fresh each time: service disappearance cannot keep a stale value.
    s = BuildFinanceSnapshot(nullptr, nullptr);
    CHECK(!s.treasury.Available() && !s.profit.effectiveFraction.Available());
}
int main() {
    MissingAndNormal();
    InvalidValues();
    PrefixesAndNullGetters();
    std::puts("almanac finance: all tests passed");
}
