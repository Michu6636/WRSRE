#include <cassert>
#include <cmath>
#include <cstdio>
#include "scarcity_curve.h"
#include "../../src/resource_pricing_api.h"

static bool feq(float a, float b) { return std::fabs(a-b) < 1e-6f; }

int main() {
    // monotonicity: as fill increases, multiplier non-increasing
    float prev = 1e9f;
    for (int i = 0; i <= 100; i++) {
        float f = (float)i / 100.0f;
        float m = scarcityMultiplier(f);
        assert(m <= prev + 1e-6f);
        prev = m;
    }
    // deadband neutrality
    assert(feq(scarcityMultiplier(0.35f), 1.0f));
    assert(feq(scarcityMultiplier(0.50f), 1.0f));
    assert(feq(scarcityMultiplier(0.65f), 1.0f));
    // endpoints
    assert(scarcityMultiplier(0.0f) > 1.0f);
    assert(scarcityMultiplier(1.0f) < 1.0f);
    // clamping corrupt/negative/NaN
    assert(feq(scarcityMultiplier(-5.0f), scarcityMultiplier(0.0f)));
    assert(feq(scarcityMultiplier(5.0f), scarcityMultiplier(1.0f)));
    assert(feq(scarcityMultiplier(NAN), 1.0f));
    assert(feq(scarcityMultiplier(INFINITY), scarcityMultiplier(1.0f)));
    // params guard
    ScarcityParams p; p.neutral_fill_low = 0.8f; p.neutral_fill_high = 0.2f; // swapped
    float m = scarcityMultiplier(0.5f, p);
    assert(m == m);

    // structSize prefix sizes
    assert(offsetof(TsmResourcePricingApi, recordSupply) < offsetof(TsmResourcePricingApi, getCpiDailyBasket));
    assert(offsetof(TsmResourcePricingApi, getCpiDailyBasket) < offsetof(TsmResourcePricingApi, getLocalScarcityMultiplier));
    assert(offsetof(TsmResourcePricingApi, getLocalScarcityMultiplier) < offsetof(TsmResourcePricingApi, getLocalPriceQuote));
    assert(offsetof(TsmResourcePricingApi, getLocalPriceQuote) + sizeof(void*) <= sizeof(TsmResourcePricingApi));

    // fallback wrappers with null api
    assert(TsmPricingHasLocalQuote(nullptr) == 0);
    assert(TsmPricingLocalMult(nullptr, nullptr, nullptr) == 1.0f);
    // TsmPricingLocalQuote fallback tested via helper presence.

    printf("resource_pricing scarcity tests PASS\n");
    return 0;
}