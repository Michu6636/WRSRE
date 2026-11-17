// Fixed-capacity, read-only Finance rows. A future native UI lays these out.
#pragma once
#include "almanac_finance_snapshot.h"
#include <cstdio>

namespace almanac {
struct FinanceRow { const char* label; char value[96]; };
struct FinanceView {
    FinanceRow rows[10];
    const char* note = "Service-reported values; sampled separately. Treasury API cannot distinguish read failure from zero.";
};
namespace finance_detail {
inline const char* MissingLabel(FinanceAvailability a) {
    switch (a) {
    case FinanceAvailability::ServiceMissing: return "Unavailable (service missing)";
    case FinanceAvailability::GetterMissing: return "Unavailable (getter missing)";
    case FinanceAvailability::NotExposed: return "Unavailable (not exposed)";
    case FinanceAvailability::ReadFailed: return "Unavailable (snapshot read failed)";
    case FinanceAvailability::InvalidValue: return "Unavailable (invalid value)";
    default: return "Unavailable";
    }
}
template<class... Args> inline void Format(char (&out)[96], const char* fmt, Args... args) {
#ifdef _WIN32
    const int n = _snprintf_s(out, sizeof(out), _TRUNCATE, fmt, args...);
#else
    const int n = std::snprintf(out, sizeof(out), fmt, args...);
#endif
    if (n < 0 || static_cast<unsigned>(n) >= sizeof(out)) {
        const char message[] = "Out of display range";
        for (unsigned i = 0; i < sizeof(message); ++i) out[i] = message[i];
    }
}
inline void Status(FinanceRow& row, const FinanceField<bool>& f) {
    Format(row.value, "%s", f.Available() ? (f.value ? "Enabled" : "Suspended") : MissingLabel(f.availability));
}
inline void Rate(FinanceRow& row, const FinanceField<double>& f) {
    if (f.Available()) Format(row.value, "%.1f%%", f.value * 100.0);
    else Format(row.value, "%s", MissingLabel(f.availability));
}
} // namespace finance_detail

inline FinanceView BuildFinanceView(const FinanceSnapshot& s) {
    using namespace finance_detail;
    FinanceView v{};
    const char* labels[] = {"Treasury (sampled active currency)", "VAT status",
        "VAT effective rate", "VAT configured rate", "Profit tax status",
        "Profit tax effective rate", "Profit tax configured rate",
        "Property tax status", "Property tax base per enterprise", "Property tax rule"};
    for (unsigned i = 0; i < 10; ++i) v.rows[i].label = labels[i];
    if (!s.activeCurrency.Available())
        Format(v.rows[0].value, "%s", MissingLabel(s.activeCurrency.availability));
    else if (!s.treasury.Available())
        Format(v.rows[0].value, "%s", MissingLabel(s.treasury.availability));
    else
        Format(v.rows[0].value, "%.2f %s", s.treasury.value,
            s.activeCurrency.value == TSM_CURRENCY_USD ? "USD" : "RUB");
    Status(v.rows[1], s.vat.enabled);
    Rate(v.rows[2], s.vat.effectiveFraction);
    Rate(v.rows[3], s.vat.configuredFraction);
    Status(v.rows[4], s.profit.enabled);
    Rate(v.rows[5], s.profit.effectiveFraction);
    Rate(v.rows[6], s.profit.configuredFraction);
    Status(v.rows[7], s.property.enabled);
    if (!s.property.baseAmountPerEnterprise.Available())
        Format(v.rows[8].value, "%s", MissingLabel(s.property.baseAmountPerEnterprise.availability));
    else if (!s.activeCurrency.Available())
        Format(v.rows[8].value, "%s", MissingLabel(s.activeCurrency.availability));
    else
        Format(v.rows[8].value, "%.2f %s / enterprise", s.property.baseAmountPerEnterprise.value,
            s.activeCurrency.value == TSM_CURRENCY_USD ? "USD" : "RUB");
    if (s.property.rule.Available())
        Format(v.rows[9].value, "%s", s.property.rule.value == PropertyTaxRule::FlatBase
            ? "Flat base" : "Base scaled by attractiveness");
    else
        Format(v.rows[9].value, "%s", MissingLabel(s.property.rule.availability));
    return v;
}
} // namespace almanac
