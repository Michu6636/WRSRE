// Eligibility for one observed-assignment DO fuel charge. No money or native reads.
#ifndef PRIVATE_LOGISTICS_ASSIGNMENT_SETTLEMENT_H
#define PRIVATE_LOGISTICS_ASSIGNMENT_SETTLEMENT_H
#include "schedule_assignment_ledger.h"

namespace private_logistics_assignment_settlement {
namespace assignment = private_logistics_assignment;

struct Assessment {
    bool billable = false;
    const char* reason = "not_closed";
};

inline Assessment Assess(const assignment::Event& event, bool discoveryComplete,
                         bool priceAvailable, double amount) {
    Assessment a;
    const auto& c = event.candidate;
    if (event.kind != assignment::Kind::Closed) return a;
    a.reason = "ambiguous_buyer";
    if (!c.originalTarget || c.originalTargetRows != 1 || !c.home) return a;
    a.reason = "unknown_start";
    if (c.unknownStart) return a;
    a.reason = "observation_gap";
    if (c.observationGap) return a;
    a.reason = "role_uncertain";
    if (c.roleUncertain) return a;
    a.reason = "fuel_uncertain";
    if (c.fuelUncertain || c.receiverChanged) return a;
    a.reason = "observed_level_rise";
    if (c.observedRise) return a;
    a.reason = "discovery_incomplete";
    if (!discoveryComplete) return a;
    a.reason = "no_compared_samples";
    if (!c.sampledPairs) return a;
    a.reason = "price_unavailable";
    if (!priceAvailable) return a;
    a.reason = "invalid_amount";
    if (!std::isfinite(amount) || amount < 0.0) return a;
    a.reason = "zero_fuel_charge";
    if (amount == 0.0) return a;
    a.billable = true;
    a.reason = "eligible_observed_assignment";
    return a;
}

} // namespace private_logistics_assignment_settlement
#endif
