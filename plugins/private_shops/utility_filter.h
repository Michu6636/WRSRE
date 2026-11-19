// utility_filter.h - pure helper for filtering non-freight utility resources
// out of private_shops storage polling (electricity, water, sewage, heating).
#ifndef WRSRE_PRIVATE_SHOPS_UTILITY_FILTER_H
#define WRSRE_PRIVATE_SHOPS_UTILITY_FILTER_H

#include <ctype.h>
#include <stddef.h>

namespace wrsre_shops {

inline bool EqualsIgnoreCaseAscii(const char* a, const char* b)
{
    if (!a || !b) return false;
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return false;
        ++a;
        ++b;
    }
    return *a == '\0' && *b == '\0';
}

// Returns true if the engine resource name identifies a non-freight utility
// buffer (electricity, water, sewage/usagewater, heating) that oscillates on
// grid/pipe ticks and must not be billed as wholesale delivery or state
// procurement.
inline bool IsUtilityResourceName(const char* name)
{
    if (!name || !name[0]) return false;
    return EqualsIgnoreCaseAscii(name, "eletric") ||
           EqualsIgnoreCaseAscii(name, "electric") ||
           EqualsIgnoreCaseAscii(name, "water") ||
           EqualsIgnoreCaseAscii(name, "usagewater") ||
           EqualsIgnoreCaseAscii(name, "heating") ||
           EqualsIgnoreCaseAscii(name, "heat") ||
           EqualsIgnoreCaseAscii(name, "hotwater");
}

} // namespace wrsre_shops

#endif // WRSRE_PRIVATE_SHOPS_UTILITY_FILTER_H
