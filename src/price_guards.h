// price_guards.h - one definition of "the game has a native price".
//
// Economy direction D2/D8/D32/D33/D34: a resource with no native price is
// UNPRICED, never a fallback. resource_pricing returns 0.0f for it, and every
// consumer uses this predicate before charging, billing or estimating. Nothing
// in the mod may turn a missing price into a number.
//
// The test matches resource_pricing's own: finite and above 0.01. The engine
// writes 0.0 for a resource it has never priced, and NaN/Inf cannot come from
// a real price field. A genuinely cheap good stays priced.
#ifndef TSM_PRICE_GUARDS_H
#define TSM_PRICE_GUARDS_H

// 1 when `price` is a native price the mod may use, 0 when it is UNPRICED.
// Kept as a C-compatible function so the Windows-only translation units and
// the portable host tests can share it without a namespace.
static inline int TsmNativePriceUsable(float price)
{
    return price == price &&          // not NaN
           price > 0.01f &&          // 0 means "the engine has no price"
           price <= 1.0e12f;         // finite, or nonsense
}

#endif // TSM_PRICE_GUARDS_H
