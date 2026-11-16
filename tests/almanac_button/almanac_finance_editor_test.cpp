#include "almanac_finance_editor.h"
#include <cstdio>
#include <cstdlib>
#include <initializer_list>
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #x); std::exit(1); } } while (0)
using namespace almanac;
static TsmTaxPolicySnapshot policy{};
static TsmTaxesApi taxes{};
static TsmCurrencyApi currency{};
static int currencyCode, reads, writes, rejectRate, rejectEnable;
static bool failRead, failAfterWrite, lie, switchCurrency;
static FinanceEditor* reenter;
struct Write { int kind; bool enabled; float value; };
static Write history[16];
static int ActiveCurrency() { return currencyCode; }
static int Read(TsmTaxPolicySnapshot* out, unsigned size) {
    CHECK(out && size >= sizeof(*out)); ++reads;
    if (failRead || (failAfterWrite && writes)) return 0;
    *out = policy;
    return 1;
}
static void Record(int kind, bool enabled, float value) {
    CHECK(kind >= 0 && kind < 3 && writes < 16);
    history[writes++] = {kind, enabled, value};
    if (reenter) {
        CHECK(reenter->Apply(&currency, &taxes).status == EditApplyStatus::Busy);
        CHECK(reenter->Load(&currency, &taxes) == EditLoadStatus::Busy);
        CHECK(!reenter->Cancel());
        CHECK(!reenter->SetValue(0, 88));
        CHECK(!reenter->SetEnabled(0, false));
    }
}
static int SetRate(int kind, float value) {
    Record(kind, false, value);
    if (kind == rejectRate) return 0;
    if (!lie) policy.configured[kind] = value;
    if (switchCurrency) currencyCode = TSM_CURRENCY_RUB;
    return 1;
}
static int SetEnabled(int kind, int enabled) {
    CHECK(enabled == 0 || enabled == 1);
    Record(kind, true, float(enabled));
    if (kind == rejectEnable) return 0;
    if (!lie) policy.enabled[kind] = enabled;
    return 1;
}
static void Reset() {
    policy = {}; policy.structSize = sizeof(policy);
    policy.configured[0] = .2f; policy.configured[1] = .15f; policy.configured[2] = 100;
    policy.enabled[0] = 0; policy.enabled[1] = policy.enabled[2] = 1;
    policy.propertyLandValue = 1;
    taxes = {}; taxes.structSize = sizeof(taxes); taxes.getPolicySnapshot = Read;
    taxes.setTaxRate = SetRate; taxes.setTaxEnabled = SetEnabled;
    currency = {}; currency.structSize = sizeof(currency); currency.getActiveCurrency = ActiveCurrency;
    currencyCode = TSM_CURRENCY_USD;
    reads = writes = 0; rejectRate = rejectEnable = -1;
    failRead = failAfterWrite = lie = switchCurrency = false; reenter = nullptr;
}
static void Load(FinanceEditor& e) {
    CHECK(e.Load(&currency, &taxes) == EditLoadStatus::Loaded && e.Active());
}
static void LocalDraftAndSuccess() {
    Reset(); FinanceEditor e;
    CHECK(e.Apply(&currency, &taxes).status == EditApplyStatus::NotLoaded);
    CHECK(!e.SetValue(0, 25));
    Load(e);
    CHECK(e.Draft().value[0] > 19.99 && !e.Draft().enabled[0]);
    CHECK(e.Apply(&currency, &taxes).status == EditApplyStatus::NoChanges);
    CHECK(!e.SetValue(-1, 2) && !e.SetEnabled(3, true));
    CHECK(e.SetValue(0, 25) && e.SetEnabled(0, true));
    CHECK(writes == 0 && policy.configured[0] == .2f);
    const int before = reads;
    CHECK(e.Cancel() && !e.Active() && reads == before && writes == 0);
    CHECK(e.Apply(&currency, &taxes).status == EditApplyStatus::NotLoaded);
    Load(e);
    CHECK(e.SetValue(0, 25) && e.SetEnabled(0, true) && e.SetValue(2, 125.5));
    reenter = &e;
    const auto r = e.Apply(&currency, &taxes);
    CHECK(r.status == EditApplyStatus::Applied && r.accepted == 3 && r.rejected == 0);
    CHECK(r.observedValid && r.observed.vat.configuredFraction.value == .25);
    CHECK(r.observed.vat.enabled.value && r.observed.property.baseAmountPerEnterprise.value == 125.5);
    CHECK(writes == 3 && history[0].kind == 0 && !history[0].enabled);
    CHECK(history[1].kind == 0 && history[1].enabled);
    CHECK(history[2].kind == 2 && history[2].value == 125.5f);
    CHECK(!e.Active() && e.Apply(&currency, &taxes).status == EditApplyStatus::NotLoaded);
}

static void ValidationAndAvailability() {
    for (int kind = 0; kind < 3; ++kind) {
        for (double bad : {-1.0, -std::numeric_limits<double>::denorm_min(),
                std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::infinity(),
                1.0e300, kind < 2 ? 100.001 : 1000000.001}) {
            Reset(); FinanceEditor e; Load(e);
            CHECK(e.SetValue(kind, bad));
            CHECK(e.SetEnabled(1, false)); // invalid value blocks ALL writes
            const auto r = e.Apply(&currency, &taxes);
            CHECK(r.status == EditApplyStatus::InvalidDraft);
            CHECK(r.invalidValues == (1u << kind) && writes == 0 && e.Active());
        }
    }
    Reset(); FinanceEditor e;
    CHECK(e.Load(&currency, nullptr) == EditLoadStatus::ServiceUnavailable);
    auto old = taxes; old.structSize = offsetof(TsmTaxesApi, getPolicySnapshot);
    CHECK(e.Load(&currency, &old) == EditLoadStatus::ServiceUnavailable);
    CHECK(e.Load(nullptr, &taxes) == EditLoadStatus::CurrencyUnavailable);
    failRead = true;
    CHECK(e.Load(&currency, &taxes) == EditLoadStatus::SnapshotUnavailable);
    failRead = false;
    policy.structSize = 0;
    CHECK(e.Load(&currency, &taxes) == EditLoadStatus::SnapshotUnavailable);
    policy.structSize = sizeof(policy);
    Load(e); CHECK(e.SetValue(0, 30));
    CHECK(e.Apply(&currency, nullptr).status == EditApplyStatus::ServiceUnavailable);
    auto missing = taxes; missing.setTaxRate = nullptr;
    CHECK(e.Apply(&currency, &missing).status == EditApplyStatus::ServiceUnavailable);
    failRead = true;
    CHECK(e.Apply(&currency, &taxes).status == EditApplyStatus::SnapshotUnavailable);
    CHECK(e.Active() && writes == 0);
    failRead = false;
    CHECK(e.Load(&currency, nullptr) == EditLoadStatus::ServiceUnavailable);
    CHECK(!e.Active()); // failed Load cannot leave an earlier draft armed
    Reset(); policy.configured[2] = 2000000;
    Load(e); CHECK(e.SetEnabled(1, false));
    CHECK(e.Apply(&currency, &taxes).status == EditApplyStatus::Applied);
    CHECK(writes == 1 && history[0].enabled && history[0].kind == 1);
    CHECK(policy.configured[2] == 2000000); // don't rewrite an unchanged high base
}

static void Conflicts() {
    for (int reason = 0; reason < 4; ++reason) {
        Reset(); FinanceEditor e; Load(e);
        CHECK(e.SetValue(0, 30));
        if (reason == 0) policy.configured[0] = .4f;
        if (reason == 1) policy.enabled[0] = 1;
        if (reason == 2) currencyCode = TSM_CURRENCY_RUB;
        if (reason == 3) { CHECK(e.SetValue(2, 150)); policy.propertyLandValue = 0; }
        const auto r = e.Apply(&currency, &taxes);
        CHECK(r.status == EditApplyStatus::Conflict && r.observedValid);
        CHECK(writes == 0 && !e.Active());
    }
    Reset(); FinanceEditor e; Load(e); CHECK(e.SetValue(0, 30));
    policy.configured[1] = .7f; // unrelated policy change is preserved, not overwritten
    const auto r = e.Apply(&currency, &taxes);
    CHECK(r.status == EditApplyStatus::Applied && writes == 1);
    CHECK(policy.configured[1] == .7f && r.observed.profit.configuredFraction.value > .69);
}

static void RejectionsAndReadback() {
    Reset(); FinanceEditor e; Load(e);
    CHECK(e.SetValue(0, 35) && e.SetEnabled(0, true));
    rejectRate = 0;
    auto r = e.Apply(&currency, &taxes);
    CHECK(r.status == EditApplyStatus::Rejected && r.accepted == 0 && r.rejected == 1);
    CHECK(r.enabled[0] == EditWriteStatus::SkippedDependency && writes == 1);
    CHECK(policy.enabled[0] == 0 && r.observedValid);
    CHECK(e.Apply(&currency, &taxes).status == EditApplyStatus::NotLoaded);
    Reset(); Load(e); CHECK(e.SetValue(0, 35) && e.SetEnabled(0, true));
    CHECK(e.SetValue(1, 40)); rejectRate = 0;
    r = e.Apply(&currency, &taxes);
    CHECK(r.status == EditApplyStatus::PartiallyApplied && r.accepted == 1 && r.rejected == 1);
    CHECK(writes == 2 && policy.configured[1] == .4f && !policy.enabled[0]);
    Reset(); Load(e); CHECK(e.SetValue(0, 35) && e.SetEnabled(0, true));
    rejectEnable = 0;
    r = e.Apply(&currency, &taxes);
    CHECK(r.status == EditApplyStatus::PartiallyApplied && r.accepted == 1 && r.rejected == 1);
    CHECK(policy.configured[0] == .35f && !policy.enabled[0]); // no rollback claim
    Reset(); Load(e); CHECK(e.SetValue(0, 35)); lie = true;
    r = e.Apply(&currency, &taxes);
    CHECK(r.status == EditApplyStatus::VerificationMismatch && r.observedValid && r.accepted == 1);
    Reset(); Load(e); CHECK(e.SetValue(0, 35)); failAfterWrite = true;
    r = e.Apply(&currency, &taxes);
    CHECK(r.status == EditApplyStatus::ReadbackFailed && !r.observedValid && r.accepted == 1);
    CHECK(!e.Active() && policy.configured[0] == .35f);
    Reset(); Load(e); CHECK(e.SetValue(0, 35)); switchCurrency = true;
    r = e.Apply(&currency, &taxes);
    CHECK(r.status == EditApplyStatus::VerificationMismatch && r.accepted == 1);
}

static void BoundariesAndIndependentSetters() {
    for (float rate : {0.0f, 1.0f, .33333334f, 0.0000001f}) {
        Reset(); policy.configured[0] = rate; policy.configured[1] = rate;
        FinanceEditor e; Load(e);
        CHECK(e.Apply(&currency, &taxes).status == EditApplyStatus::NoChanges);
        CHECK(writes == 0); // percent/fraction round trip must not dirty the draft
    }
    for (double percent : {0.0, 100.0}) {
        Reset(); FinanceEditor e; Load(e);
        CHECK(e.SetValue(0, percent) && e.SetValue(1, percent));
        CHECK(e.SetValue(2, percent == 0 ? 0 : 1000000));
        CHECK(e.Apply(&currency, &taxes).status == EditApplyStatus::Applied);
        CHECK(policy.configured[0] == float(percent / 100));
        CHECK(policy.configured[2] == (percent == 0 ? 0 : 1000000));
    }
    Reset(); FinanceEditor e; Load(e); CHECK(e.SetEnabled(0, true));
    auto missing = taxes; missing.setTaxEnabled = nullptr;
    CHECK(e.Apply(&currency, &missing).status == EditApplyStatus::ServiceUnavailable);
    CHECK(writes == 0);
    missing = taxes; missing.setTaxRate = nullptr;
    CHECK(e.Apply(&currency, &missing).status == EditApplyStatus::Applied);
    CHECK(writes == 1 && history[0].enabled);
    Reset(); Load(e); CHECK(e.SetValue(0, 30));
    missing = taxes; missing.setTaxEnabled = nullptr;
    CHECK(e.Apply(&currency, &missing).status == EditApplyStatus::Applied);
    CHECK(writes == 1 && !history[0].enabled);
}

int main() {
    LocalDraftAndSuccess();
    BoundariesAndIndependentSetters();
    ValidationAndAvailability();
    Conflicts();
    RejectionsAndReadback();
    std::puts("almanac finance editor: all tests passed");
}
