// cpi_rent.h - choosing the daily rent term of the CPI basket.
//
// Economy direction D35: the RUB and USD default rents are two documented INI
// constants, not one number divided by a rate. Native dollar prices are of the
// same size as rouble prices (usually 10-30% higher), so a 50:1 divisor is not
// an exchange rate and must never be applied.
//
// Both keys are REQUIRED and behave the same way. A missing key, 0, a negative
// value, NaN or an implausible value is invalid: the basket is UNAVAILABLE
// (D8/D34/D35). There is no compiled-in default and no "no rent" setting.
//
// The Windows provider cannot be compiled on the host, so this decision lives
// here and is tested directly by tests/resource_pricing/test_no_fallbacks.py.
#ifndef WRSRE_RESOURCE_PRICING_CPI_RENT_H
#define WRSRE_RESOURCE_PRICING_CPI_RENT_H

#include "../../src/currency_api.h"

namespace wrsre {
namespace pricing {

struct CpiRent {
    bool        available;    // false = no usable rent, so the basket is UNAVAILABLE
    float       value;        // rent per day, in the active currency
    const char* missingKey;   // nullptr, or the INI key that was missing/invalid
};

// injected > 0 is a Level 2 caller's real rent and always wins. A zero or
// invalid injection is ignored and the currency's own REQUIRED key decides.
inline CpiRent ChooseCpiRent(int currency, float injected,
                             float rubDefault, float usdDefault)
{
    const auto usable = [](float v) {
        return v == v && v > 0.0f && v <= 10000.0f;
    };

    CpiRent out = { false, 0.0f, nullptr };
    if (injected == injected && injected > 0.0f && injected <= 10000.0f) {
        out.available = true;
        out.value     = injected;
        return out;
    }
    if (currency == TSM_CURRENCY_USD) {
        if (!usable(usdDefault)) {
            out.missingKey = "default_daily_rent_usd";
            return out;
        }
        out.available = true;
        out.value     = usdDefault;
        return out;
    }
    if (!usable(rubDefault)) {
        out.missingKey = "default_daily_rent";
        return out;
    }
    out.available = true;
    out.value     = rubDefault;
    return out;
}

} // namespace pricing
} // namespace wrsre

#endif // WRSRE_RESOURCE_PRICING_CPI_RENT_H
