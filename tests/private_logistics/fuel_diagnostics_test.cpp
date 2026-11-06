// Candidate provenance tests. Not proof of native offsets, units, or events.
#include "fuel_diagnostics.h"
#include <cassert>
#include <cstdio>
#include <limits>
#include <initializer_list>
using namespace private_logistics_fuel_diagnostics;
int main() {
    const auto fallback=Select(false,-1.0,0.09);
    assert(fallback.valid && fallback.source==Source::DisplayFallback && !fallback.comparable);
    std::puts("PASS display_fallback_not_self_comparable");
    auto slot=Select(true,0.08,0.09);
    assert(slot.valid && slot.source==Source::SlotCandidate && slot.comparable);
    assert(std::fabs(slot.drift-0.01)<1e-12);
    std::puts("PASS independent_candidate_comparison");
    for(double invalid : {-1.0, -0.03, 1001.0, std::numeric_limits<double>::infinity(),
                          std::numeric_limits<double>::quiet_NaN()}) {
        assert(!Select(false,-1.0,invalid).valid);
        assert(!Select(true,invalid,0.09).valid); // found-invalid slot cannot be hidden
    }
    std::puts("PASS invalid_negative_and_nonfinite_not_zero_or_hidden");
    slot=Select(true,0.0,-1.0);assert(slot.valid && !slot.comparable && slot.value==0.0);
    assert(Select(false,0.0,0.0).valid);
    std::puts("PASS readable_zero_is_valid_invalid_is_not_zero");
    Samples samples;
    assert(samples.Observe(fallback,1).change==Change::Baseline);
    auto r=samples.Observe(Select(false,-1.0,0.07),1);
    assert(r.change==Change::Decrease && std::fabs(r.decrease-0.02)<1e-12);
    assert(samples.Observe(Select(false,-1.0,0.08),1).change==Change::Increase);
    assert(samples.Observe(Select(false,-1.0,0.08),1).change==Change::Flat);
    std::puts("PASS changes_are_sample_decreases_and_increases_only");
    r=samples.Observe(Select(true,0.01,0.08),1);
    assert(r.change==Change::SourceChange && r.decrease==0.0);
    r=samples.Observe(Select(false,-1.0,0.005),1);
    assert(r.change==Change::SourceChange && r.decrease==0.0);
    std::puts("PASS source_changes_reset_baseline_in_both_directions");
    assert(samples.Observe(Select(false,-1.0,-0.03),1).change==Change::Invalid);
    r=samples.Observe(Select(false,-1.0,0.001),1);
    assert(r.change==Change::Baseline && r.decrease==0.0);
    std::puts("PASS invalid_read_breaks_sample_continuity");
    r=samples.Observe(Select(false,-1.0,0.0),2);
    assert(r.change==Change::Baseline && r.decrease==0.0);
    std::puts("PASS load_epoch_breaks_sample_continuity");
    std::puts("8 candidate-provenance scenarios PASS (not native fuel validation)");
}
