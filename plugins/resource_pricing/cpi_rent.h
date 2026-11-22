// cpi_rent.h - choosing the daily rent term of the CPI basket.
//
// Economy direction D35: the RUB and USD default rents are two documented INI
// constants, not one number divided by a rate. Native dollar prices are of the
// same size as rouble prices (usually 10-30% higher), so a 50:1 divisor is not
// an exchange rate and must never be applied. The USD key is optional only in
// the sense that RUB mode does not read it; when USD mode needs it and it is
// missing or invalid, the basket is UNAVAILABLE (D8/D34).
//
// The Windows provider cannot be compiled on the host, so this decision lives
// here and is tested directly by tests/resource_pricing/test_no_fallbacks.py.
#ifndef WRSRE_RESOURCE_PRICING_CPI_RENT_H
#define WRSRE_RESOURCE_PRICING_CPI_RENT_H

#include "../../src/currency_api.h"

namespace wrsre {
namespace pricing {

struct CpiRent {
    bool  available;      // false = no usable rent, so the basket is UNAVAILABLE
    float value;          // rent per day, in the active currency
    bool  usdKeyMissing;  // true when USD mode had no usable default_daily_rent_usd
};

// injected > 0 is a Level 2 caller's real rent and always wins.
inline CpiRent ChooseCpiRent(int currency, float injected,
                             float rubDefault, float usdDefault)
{
    const auto usable = [](float v) {
        return v == v && v > 0.0f && v <= 10000.0f;
    };

    CpiRent out = { false, 0.0f, false };
    if (injected == injected && injected > 0.0f && injected <= 10000.0f) {
        out.available = true;
        out.value     = injected;
        return out;
    }
    if (currency == TSM_CURRENCY_USD) {
        if (!usable(usdDefault)) {
            out.usdKeyMissing = true;
            return out;
        }
        out.available = true;
        out.value     = usdDefault;
        return out;
    }
    if (!usable(rubDefault)) return out;
    out.available = true;
    out.value     = rubDefault;
    return out;
}

} // namespace pricing
} // namespace wrsre

#endif // WRSRE_RESOURCE_PRICING_CPI_RENT_H
