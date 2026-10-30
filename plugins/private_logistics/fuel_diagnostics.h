// Legacy candidate readers: provenance and sampled changes ONLY, never burn events.
#ifndef PRIVATE_LOGISTICS_FUEL_DIAGNOSTICS_H
#define PRIVATE_LOGISTICS_FUEL_DIAGNOSTICS_H
#include <cmath>
#include <cstdint>
namespace private_logistics_fuel_diagnostics {
enum class Source { Unavailable, SlotCandidate, DisplayFallback };
enum class Change { Invalid, Baseline, SourceChange, Decrease, Increase, Flat };
struct Reading {
    Source source = Source::Unavailable;
    bool valid = false, comparable = false;
    double value = 0.0, drift = 0.0;
};
inline bool Candidate(double value) {
    return std::isfinite(value) && value >= 0.0 && value <= 1000.0;
}
inline Reading Select(bool slotFound, double slotRaw, double displayRaw) {
    Reading r;
    // A found but INVALID slot must not be hidden by a valid fallback.
    if (slotFound) {
        r.source = Source::SlotCandidate;
        r.valid = Candidate(slotRaw);
        if (r.valid) r.value = slotRaw;
        r.comparable = r.valid && Candidate(displayRaw);
        if (r.comparable) r.drift = displayRaw - slotRaw;
    } else if (Candidate(displayRaw)) {
        r.source = Source::DisplayFallback;
        r.valid = true;
        r.value = displayRaw;
        // Never compare a fallback to itself and call it agreement.
    }
    return r;
}
inline const char* SourceName(Source s) {
    switch (s) {
        case Source::SlotCandidate: return "slot_candidate";
        case Source::DisplayFallback: return "display_5f0_fallback";
        default: return "unavailable";
    }
}
inline const char* ChangeName(Change c) {
    switch (c) {
        case Change::Baseline: return "baseline";
        case Change::SourceChange: return "source_change_baseline";
        case Change::Decrease: return "sample_decrease";
        case Change::Increase: return "sample_increase_not_verified_refill";
        case Change::Flat: return "sample_unchanged";
        default: return "invalid";
    }
}
struct Result { Change change = Change::Invalid; double decrease = 0.0; };
class Samples {
    bool known_ = false;
    std::uint64_t generation_ = 0;
    Source source_ = Source::Unavailable;
    double previous_ = 0.0;
public:
    Result Observe(const Reading& r, std::uint64_t generation) {
        Result result;
        if (!r.valid) { known_ = false; return result; }
        if (!known_ || generation != generation_) result.change = Change::Baseline;
        else if (source_ != r.source) result.change = Change::SourceChange;
        else if (r.value < previous_) {
            result.change = Change::Decrease; result.decrease = previous_ - r.value;
        } else result.change = r.value > previous_ ? Change::Increase : Change::Flat;
        generation_ = generation; source_ = r.source; previous_ = r.value; known_ = true;
        return result;
    }
};
}
#endif
