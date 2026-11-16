// Read-only presentation model. No game offsets, writes, allocation or UI ABI.
#pragma once
#include "currency_api.h"
#include "taxes_api.h"
#include <cmath>

namespace almanac {
enum class FinanceAvailability {
    Available, ServiceMissing, GetterMissing, NotExposed, InvalidValue, ReadFailed
};
template<class T> struct FinanceField {
    T value{}; // Never display unless Available(). Zero is a legitimate value.
    FinanceAvailability availability = FinanceAvailability::GetterMissing;
    bool Available() const { return availability == FinanceAvailability::Available; }
};
struct RateSnapshot {
    FinanceField<bool> enabled;
    FinanceField<double> effectiveFraction;
    FinanceField<double> configuredFraction;
};
enum class PropertyTaxRule { FlatBase, AttractivenessScaled };
struct PropertySnapshot {
    FinanceField<bool> enabled;
    // Active-currency base amount PER ENTERPRISE, never a percentage or total.
    FinanceField<double> baseAmountPerEnterprise;
    FinanceField<PropertyTaxRule> rule;
};
struct FinanceSnapshot {
    FinanceField<int> activeCurrency;
    // Balance in activeCurrency sampled above; neither field is a native handle.
    FinanceField<double> treasury;
    RateSnapshot vat;
    RateSnapshot profit;
    PropertySnapshot property;
};

namespace finance_detail {
template<class T> inline FinanceField<T> Missing(FinanceAvailability reason) {
    return {T{}, reason};
}
template<class T> inline FinanceField<T> Value(T value) {
    return {value, FinanceAvailability::Available};
}
inline FinanceField<double> ReadRate(float (*getter)()) {
    if (!getter) return Missing<double>(FinanceAvailability::GetterMissing);
    const double value = getter();
    if (!std::isfinite(value) || value < 0.0 || value > 1.0)
        return Missing<double>(FinanceAvailability::InvalidValue);
    return Value(value);
}
inline void UnavailablePolicy(FinanceSnapshot& s, FinanceAvailability reason) {
    s.vat.enabled = s.profit.enabled = s.property.enabled = Missing<bool>(reason);
    s.vat.configuredFraction = s.profit.configuredFraction = Missing<double>(reason);
    s.vat.effectiveFraction = s.profit.effectiveFraction = Missing<double>(reason);
    s.property.baseAmountPerEnterprise = Missing<double>(reason);
    s.property.rule = Missing<PropertyTaxRule>(reason);
}
inline void ReadPolicy(FinanceSnapshot& s, const TsmTaxesApi* api) {
    TsmTaxPolicySnapshot p{};
    if (!api->getPolicySnapshot(&p, sizeof(p))) {
        UnavailablePolicy(s, FinanceAvailability::ReadFailed);
        return;
    }
    bool valid = p.structSize >= sizeof(p) &&
        (p.propertyLandValue == 0 || p.propertyLandValue == 1);
    for (unsigned i = 0; i < 3; ++i) {
        valid = valid && std::isfinite(p.configured[i]) && p.configured[i] >= 0.0f &&
            (i == TSM_TAX_PROPERTY || p.configured[i] <= 1.0f) &&
            (p.enabled[i] == 0 || p.enabled[i] == 1);
    }
    if (!valid) {
        UnavailablePolicy(s, FinanceAvailability::InvalidValue);
        return;
    }
    s.vat.enabled = Value(p.enabled[TSM_TAX_VAT] != 0);
    s.profit.enabled = Value(p.enabled[TSM_TAX_PROFIT] != 0);
    s.property.enabled = Value(p.enabled[TSM_TAX_PROPERTY] != 0);
    s.vat.configuredFraction = Value(double(p.configured[TSM_TAX_VAT]));
    s.profit.configuredFraction = Value(double(p.configured[TSM_TAX_PROFIT]));
    s.vat.effectiveFraction = Value(p.enabled[TSM_TAX_VAT] ? double(p.configured[TSM_TAX_VAT]) : 0.0);
    s.profit.effectiveFraction = Value(p.enabled[TSM_TAX_PROFIT] ? double(p.configured[TSM_TAX_PROFIT]) : 0.0);
    s.property.baseAmountPerEnterprise = Value(double(p.configured[TSM_TAX_PROPERTY]));
    s.property.rule = Value(p.propertyLandValue ? PropertyTaxRule::AttractivenessScaled : PropertyTaxRule::FlatBase);
}
// Deliberately per-member guards, including prefix members. Do not require
// write-policy callbacks for a read-only enabled-state getter.
#define ALMANAC_FINANCE_HAS(type, api, member) \
    ((api) && (api)->structSize >= offsetof(type, member) + sizeof((api)->member) && (api)->member)
} // namespace finance_detail

// Caller resolves exact versions (currency v4/v3; taxes v3/v2/v1) and keeps
// services alive during this call. v3 tax policy is coherent; legacy tax and
// currency reads are separate, NOT an atomic tax/treasury snapshot.
inline FinanceSnapshot BuildFinanceSnapshot(const TsmCurrencyApi* currency,
                                             const TsmTaxesApi* taxes) {
    using namespace finance_detail;
    using A = FinanceAvailability;
    FinanceSnapshot s;
    if (!currency) {
        s.activeCurrency = Missing<int>(A::ServiceMissing);
        s.treasury = Missing<double>(A::ServiceMissing);
    } else if (!ALMANAC_FINANCE_HAS(TsmCurrencyApi, currency, getActiveCurrency)) {
        s.activeCurrency = Missing<int>(A::GetterMissing);
        s.treasury = Missing<double>(A::GetterMissing);
    } else {
        const int code = currency->getActiveCurrency();
        if (code != TSM_CURRENCY_RUB && code != TSM_CURRENCY_USD) {
            s.activeCurrency = Missing<int>(A::InvalidValue);
            s.treasury = Missing<double>(A::InvalidValue);
        } else {
            s.activeCurrency = Value(code);
            if (ALMANAC_FINANCE_HAS(TsmCurrencyApi, currency, getTreasuryBalance)) {
                const double amount = currency->getTreasuryBalance(code);
                s.treasury = std::isfinite(amount) ? Value(amount) : Missing<double>(A::InvalidValue);
            }
        }
    }
    const A notExposed = taxes ? A::NotExposed : A::ServiceMissing;
    s.vat.configuredFraction = Missing<double>(notExposed);
    s.profit.configuredFraction = Missing<double>(notExposed);
    s.property.baseAmountPerEnterprise = Missing<double>(notExposed);
    s.property.rule = Missing<PropertyTaxRule>(notExposed);
    if (!taxes) {
        s.vat.enabled = s.profit.enabled = s.property.enabled = Missing<bool>(A::ServiceMissing);
        s.vat.effectiveFraction = s.profit.effectiveFraction = Missing<double>(A::ServiceMissing);
        return s;
    }
    if (TsmTaxesHasPolicySnapshot(taxes)) {
        ReadPolicy(s, taxes);
        return s; // Do not mix a failed v3 snapshot with sequential old getters.
    }
    if (ALMANAC_FINANCE_HAS(TsmTaxesApi, taxes, getVatRate))
        s.vat.effectiveFraction = ReadRate(taxes->getVatRate);
    if (ALMANAC_FINANCE_HAS(TsmTaxesApi, taxes, getProfitTaxRate))
        s.profit.effectiveFraction = ReadRate(taxes->getProfitTaxRate);
    if (ALMANAC_FINANCE_HAS(TsmTaxesApi, taxes, isTaxEnabled)) {
        FinanceField<bool>* enabled[] = {&s.vat.enabled, &s.profit.enabled, &s.property.enabled};
        const int kinds[] = {TSM_TAX_VAT, TSM_TAX_PROFIT, TSM_TAX_PROPERTY};
        for (unsigned i = 0; i < 3; ++i) {
            const int value = taxes->isTaxEnabled(kinds[i]);
            *enabled[i] = (value == 0 || value == 1) ? Value(value == 1) : Missing<bool>(A::InvalidValue);
        }
    }
    return s;
}
#undef ALMANAC_FINANCE_HAS
} // namespace almanac
