// scarcity_curve.h - pure math for local per-building inventory scarcity
// quote spreads. No game/native dependencies; safe for offline unit tests.
#ifndef TSM_SCARCITY_CURVE_H
#define TSM_SCARCITY_CURVE_H

#include <cmath>

struct ScarcityParams {
    float empty_markup_max  = 1.30f;
    float full_discount_min = 0.82f;
    float neutral_fill_low   = 0.35f;
    float neutral_fill_high = 0.65f;
    float min_capacity_tons = 0.5f;
};

namespace scarcity_detail {
inline bool isFinite(float v) { return v == v; }
}

inline float scarcityMultiplier(float fillRatio, const ScarcityParams& p = ScarcityParams())
{
    // Guard NaN/Inf in fill ratio.
    if (!(fillRatio == fillRatio)) return 1.0f;
    if (fillRatio < 0.0f) fillRatio = 0.0f;
    if (fillRatio > 1.0f) fillRatio = 1.0f;

    // Guard corrupt params.
    float low  = p.neutral_fill_low;
    float high = p.neutral_fill_high;
    if (!(low == low) || !(high == high)) { low = 0.35f; high = 0.65f; }
    if (low < 0.0f) low = 0.0f;
    if (high > 1.0f) high = 1.0f;
    if (low > high) { float t = low; low = high; high = t; }

    float emptyMax = p.empty_markup_max;
    float fullMin  = p.full_discount_min;
    if (!(emptyMax == emptyMax) || emptyMax < 1.0f) emptyMax = 1.0f;
    if (!(fullMin == fullMin) || fullMin > 1.0f) fullMin = 1.0f;
    if (fullMin > emptyMax) fullMin = emptyMax;

    // Neutral deadband.
    if (fillRatio >= low && fillRatio <= high) return 1.0f;

    if (fillRatio < low) {
        // Interpolate from emptyMax at 0 to 1.0 at low.
        float t = (low > 0.0f) ? (fillRatio / low) : 1.0f;
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        return emptyMax + (1.0f - emptyMax) * t;
    }
    // fillRatio > high: interpolate from 1.0 at high to fullMin at 1.
    float denom = 1.0f - high;
    float t = (denom > 0.0f) ? ((fillRatio - high) / denom) : 1.0f;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return 1.0f + (fullMin - 1.0f) * t;
}

inline float scarcityMultiplier(float fillRatio,
                               float emptyMax, float fullMin,
                               float low, float high)
{
    ScarcityParams p;
    p.empty_markup_max  = emptyMax;
    p.full_discount_min = fullMin;
    p.neutral_fill_low   = low;
    p.neutral_fill_high  = high;
    return scarcityMultiplier(fillRatio, p);
}

// Aliases for test/compatibility.
inline float scarcityMult(float fillRatio, const ScarcityParams& p = ScarcityParams()) { return scarcityMultiplier(fillRatio, p); }
inline float scarcityMult(float fillRatio, float emptyMax, float fullMin, float low, float high) { return scarcityMultiplier(fillRatio, emptyMax, fullMin, low, high); }
inline float localScarcityMultiplier(float fillRatio, const ScarcityParams& p = ScarcityParams()) { return scarcityMultiplier(fillRatio, p); }
inline float localScarcityMultiplier(float fillRatio, float emptyMax, float fullMin, float low, float high) { return scarcityMultiplier(fillRatio, emptyMax, fullMin, low, high); }
inline float computeScarcityMultiplier(float fillRatio, const ScarcityParams& p = ScarcityParams()) { return scarcityMultiplier(fillRatio, p); }
inline float computeScarcityMultiplier(float fillRatio, float emptyMax, float fullMin, float low, float high) { return scarcityMultiplier(fillRatio, emptyMax, fullMin, low, high); }
inline float TsmScarcityMultiplier(float fillRatio, const ScarcityParams& p = ScarcityParams()) { return scarcityMultiplier(fillRatio, p); }
inline float TsmScarcityMultiplier(float fillRatio, float emptyMax, float fullMin, float low, float high) { return scarcityMultiplier(fillRatio, emptyMax, fullMin, low, high); }

#endif
