// checkout_policy.h - the portable part of the checkout price decision.
//
// Economy direction D32: when a shop good has no native price, the game gives
// the good as in vanilla and no money moves. The configured sale floor
// (min_cost) must NOT turn that missing price into a charge.
//
// The Windows checkout hook cannot be compiled on the host because it uses
// MSVC SEH, so the arithmetic lives here and is tested directly by
// tests/citizen_retail/test_unpriced_sale.py.
#ifndef WRSRE_CITIZEN_RETAIL_CHECKOUT_POLICY_H
#define WRSRE_CITIZEN_RETAIL_CHECKOUT_POLICY_H

#include "../../src/price_guards.h"

namespace wrsre {
namespace retail {

struct CheckoutQuote {
    bool  priced;   // false = UNPRICED: allow the vanilla give, charge nothing
    float cost;     // amount x native unit price, raised to the sale floor
};

// pricePerTon comes from resource_pricing (0 = no native price).
// amount is the tonnes the engine is about to dispense.
// minCost is the configured sale floor; it applies only to a priced sale.
inline CheckoutQuote QuoteSale(float pricePerTon, float amount, float minCost)
{
    CheckoutQuote q = { false, 0.0f };
    if (!(amount > 0.0f) || !(amount == amount)) return q;
    if (!TsmNativePriceUsable(pricePerTon)) return q;

    float cost = amount * pricePerTon;
    if (!(cost == cost) || cost < 0.0f) cost = 0.0f;
    if (minCost > 0.0f && cost < minCost) cost = minCost;

    q.priced = true;
    q.cost   = cost;
    return q;
}

} // namespace retail
} // namespace wrsre

#endif // WRSRE_CITIZEN_RETAIL_CHECKOUT_POLICY_H
